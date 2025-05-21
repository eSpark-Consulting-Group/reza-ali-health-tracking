#include "rr_peak_detector.h"

#define PEAK_THRESHOLD 2000

static int16_t last_sample = 0;
static uint32_t last_peak_time = 0;
static uint32_t sample_interval_ms = 8;

bool rr_detect_peak(int16_t sample, uint32_t timestamp_ms, uint16_t *rr_out) {
    if (sample > PEAK_THRESHOLD && last_sample <= PEAK_THRESHOLD) {
        if (last_peak_time > 0) {
            *rr_out = (uint16_t)(timestamp_ms - last_peak_time);
            last_peak_time = timestamp_ms;
            last_sample = sample;
            return true;
        }
        last_peak_time = timestamp_ms;
    }
    last_sample = sample;
    return false;
}
