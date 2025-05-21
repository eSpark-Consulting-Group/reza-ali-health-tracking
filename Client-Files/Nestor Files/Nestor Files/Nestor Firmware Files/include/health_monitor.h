#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#include <stdbool.h>

void health_monitor_record_temp(float temp);
float health_monitor_compute_temp_zscore(float latest_temp);
bool health_monitor_check_fever_alert(float temp_z, float hrv_rmssd);

#endif
