#include "ppg_signal_monitor.h"
#include "sqi_filter.h"

bool ppg_signal_ok(const int16_t *samples, int count) {
    return sqi_is_acceptable(samples, count);
}
