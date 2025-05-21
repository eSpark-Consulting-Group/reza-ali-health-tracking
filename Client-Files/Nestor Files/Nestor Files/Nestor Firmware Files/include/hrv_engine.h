#ifndef HRV_ENGINE_H
#define HRV_ENGINE_H

#include <stdint.h>

void hrv_engine_add_interval(uint16_t rr);
float hrv_engine_compute_rmssd(void);
float hrv_engine_compute_sdnn(void);
void hrv_engine_clear(void);

#endif
