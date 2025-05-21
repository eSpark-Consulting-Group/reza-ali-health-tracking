#include "sqi_filter.h"
#include <math.h>
#include <stdint.h>

float sqi_filter_process(uint16_t *signal, uint16_t length) {
    if (length < 2) return 0.0f;

    float mean = 0.0f;
    for (int i = 0; i < length; i++) mean += signal[i];
    mean /= length;

    float variance = 0.0f;
    for (int i = 0; i < length; i++) {
        float diff = signal[i] - mean;
        variance += diff * diff;
    }
    variance /= length;

    return fminf(1.0f, variance / 500000.0f); // Normalize to 0.0 - 1.0
}
