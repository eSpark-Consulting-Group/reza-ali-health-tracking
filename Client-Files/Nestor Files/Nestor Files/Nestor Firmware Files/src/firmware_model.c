#include "firmware_model.h"
#include <math.h>
#include <string.h>

#define MAX_RR_COUNT 64
#define KALMAN_GAIN 0.2f
#define EMA_ALPHA  0.2f

static uint16_t rr_buffer[MAX_RR_COUNT];
static uint8_t rr_index = 0;
static float hr_filtered = 0.0f;
static float temp_ema = 36.5f;
static float baseline_temp = 36.5f;

static float last_valid_rmssd = 0.0f;
static uint8_t motion_level = 0;
static float temp_history[14];
static uint8_t temp_ptr = 0;
static uint8_t hr_raw = 0;

void firmware_model_init(void) {
    memset(rr_buffer, 0, sizeof(rr_buffer));
    memset(temp_history, 0, sizeof(temp_history));
}

void firmware_model_add_rr_interval(uint16_t rr_ms) {
    rr_buffer[rr_index++] = rr_ms;
    if (rr_index >= MAX_RR_COUNT) rr_index = 0;
}

void firmware_model_record_hr(uint8_t hr) {
    hr_raw = hr;
    hr_filtered = (1 - KALMAN_GAIN) * hr_filtered + KALMAN_GAIN * hr;
}

void firmware_model_record_motion(uint8_t level) {
    motion_level = level;
}

void firmware_model_record_temperature(float temp_c) {
    temp_history[temp_ptr++] = temp_c;
    if (temp_ptr >= 14) temp_ptr = 0;
    temp_ema = EMA_ALPHA * temp_c + (1 - EMA_ALPHA) * temp_ema;
}

float firmware_model_get_temp_ema(void) {
    return temp_ema;
}

static float compute_rmssd(void) {
    if (rr_index < 3) return last_valid_rmssd;
    float sum_sq = 0.0f;
    for (int i = 1; i < rr_index; ++i) {
        float diff = rr_buffer[i] - rr_buffer[i-1];
        sum_sq += diff * diff;
    }
    last_valid_rmssd = sqrtf(sum_sq / (rr_index - 1));
    return last_valid_rmssd;
}

uint8_t firmware_model_compute_readiness(void) {
    float rmssd = compute_rmssd();
    float score = (rmssd > 50.0f) ? 100.0f : (rmssd * 2.0f);
    if (motion_level >= 3) score -= 10;  // high strain penalty
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    return (uint8_t)score;
}

bool firmware_model_detect_fever(void) {
    float temp_z = (temp_ema - baseline_temp) / 0.2f;
    return (temp_z > 2.0f && last_valid_rmssd < 25.0f);
}

bool firmware_model_estimate_stress(void) {
    return (last_valid_rmssd < 20.0f && hr_filtered > 90.0f);
}
