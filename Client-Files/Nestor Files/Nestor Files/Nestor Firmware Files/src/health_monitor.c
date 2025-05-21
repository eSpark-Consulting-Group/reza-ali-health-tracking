#include "health_monitor.h"
#include "hrv_engine.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define TEMP_BUFFER_SIZE 14
#define ACTIVITY_BUFFER_SIZE 7
#define Z_THRESHOLD 2.0f
#define HR_HIGH_THRESHOLD 90
#define HRV_LOW_THRESHOLD 20

static float temp_history[TEMP_BUFFER_SIZE];
static int temp_index = 0;
static int temp_count = 0;

static float activity_buffer[ACTIVITY_BUFFER_SIZE];
static int activity_index = 0;
static int activity_count = 0;

void health_monitor_record_temp(float temp) {
    temp_history[temp_index] = temp;
    temp_index = (temp_index + 1) % TEMP_BUFFER_SIZE;
    if (temp_count < TEMP_BUFFER_SIZE) temp_count++;
}

void health_monitor_record_activity(float score) {
    activity_buffer[activity_index] = score;
    activity_index = (activity_index + 1) % ACTIVITY_BUFFER_SIZE;
    if (activity_count < ACTIVITY_BUFFER_SIZE) activity_count++;
}

float health_monitor_compute_temp_zscore(float latest_temp) {
    if (temp_count < 2) return 0.0f;
    float mean = 0.0f;
    for (int i = 0; i < temp_count; ++i) mean += temp_history[i];
    mean /= temp_count;

    float stddev = 0.0f;
    for (int i = 0; i < temp_count; ++i)
        stddev += (temp_history[i] - mean) * (temp_history[i] - mean);
    stddev = sqrtf(stddev / temp_count);

    return (latest_temp - mean) / stddev;
}

bool health_monitor_check_fever_alert(float temp_z, float hrv_rmssd) {
    return (temp_z > Z_THRESHOLD && hrv_rmssd < HRV_LOW_THRESHOLD);
}

uint8_t health_monitor_estimate_stress(uint8_t resting_hr, float hrv_rmssd) {
    if (resting_hr > HR_HIGH_THRESHOLD && hrv_rmssd < HRV_LOW_THRESHOLD)
        return 80 + (resting_hr - HR_HIGH_THRESHOLD) / 2;
    else if (hrv_rmssd < HRV_LOW_THRESHOLD)
        return 60;
    else
        return 30;
}

uint8_t health_monitor_compute_readiness(uint8_t resting_hr, float hrv_rmssd, float temp_z, float recent_activity) {
    int score = 100;
    if (resting_hr > 80) score -= 10;
    if (hrv_rmssd < 30) score -= 20;
    if (temp_z > 1.5f) score -= 10;
    if (recent_activity > 70) score -= 10;

    if (score < 0) score = 0;
    return (uint8_t)score;
}

float health_monitor_recent_activity_avg(void) {
    if (activity_count == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < activity_count; ++i) sum += activity_buffer[i];
    return sum / activity_count;
}
