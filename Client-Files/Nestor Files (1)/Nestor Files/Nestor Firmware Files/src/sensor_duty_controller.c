#include "sensor_duty_controller.h"
#include <stdbool.h>

static bool sampling_enabled = true;

void sensor_duty_controller_init(void) {
    sampling_enabled = true;
}

void sensor_duty_controller_update(uint8_t motion_level) {
    sampling_enabled = (motion_level > 5);
}

bool sensor_duty_controller_sampling_enabled(void) {
    return sampling_enabled;
}
