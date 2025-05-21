#include "hrv_engine.h"
#include <math.h>
#include <string.h>

#define MAX_RR_BUFFER 64

static uint16_t rr_intervals[MAX_RR_BUFFER];
static int rr_count = 0;

void hrv_engine_add_interval(uint16_t rr) {
    if (rr_count < MAX_RR_BUFFER) {
        rr_intervals[rr_count++] = rr;
    } else {
        memmove(rr_intervals, rr_intervals + 1, (MAX_RR_BUFFER - 1) * sizeof(uint16_t));
        rr_intervals[MAX_RR_BUFFER - 1] = rr;
    }
}

float hrv_engine_compute_rmssd(void) {
    if (rr_count < 2) return 0.0f;
    float sum = 0.0f;
    for (int i = 1; i < rr_count; ++i) {
        float diff = rr_intervals[i] - rr_intervals[i - 1];
        sum += diff * diff;
    }
    return sqrtf(sum / (rr_count - 1));
}

float hrv_engine_compute_sdnn(void) {
    if (rr_count == 0) return 0.0f;
    float mean = 0.0f;
    for (int i = 0; i < rr_count; ++i) mean += rr_intervals[i];
    mean /= rr_count;

    float sum_sq = 0.0f;
    for (int i = 0; i < rr_count; ++i)
        sum_sq += (rr_intervals[i] - mean) * (rr_intervals[i] - mean);

    return sqrtf(sum_sq / rr_count);
}

uint8_t hrv_engine_get_readiness_score(void) {
    float rmssd = hrv_engine_compute_rmssd();
    if (rmssd < 20) return 30;
    if (rmssd < 40) return 60;
    if (rmssd < 60) return 80;
    return 100;
}

void hrv_engine_clear(void) {
    rr_count = 0;
}
