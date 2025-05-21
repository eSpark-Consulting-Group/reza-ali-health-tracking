#ifndef SENSOR_DUTY_CONTROLLER_H
#define SENSOR_DUTY_CONTROLLER_H

#include <stdint.h>
#include <stdbool.h>

void sensor_duty_controller_init(void);
void sensor_duty_controller_update(uint8_t motion_level);
bool sensor_duty_controller_sampling_enabled(void);

#endif
