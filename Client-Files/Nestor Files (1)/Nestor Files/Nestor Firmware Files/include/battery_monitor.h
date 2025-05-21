#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>

void battery_monitor_init(void);
uint8_t battery_get_percent(void);
float battery_get_voltage(void);

#endif
