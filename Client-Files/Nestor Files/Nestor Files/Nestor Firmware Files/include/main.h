#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

void sensor_timeout_handler(void *p_context);
void health_timeout_handler(void *p_context);
void timers_init(void);
void buttons_init(void);
void log_init(void);
void power_manage(void);

#endif // MAIN_H
