#ifndef FIRMWARE_MODEL_H
#define FIRMWARE_MODEL_H

#include <stdint.h>
#include <stdbool.h>

void firmware_model_init(void);

void firmware_model_add_rr_interval(uint16_t rr_ms);
void firmware_model_record_temperature(float temp_c);
void firmware_model_record_motion(uint8_t motion_level);
void firmware_model_record_hr(uint8_t hr);

uint8_t firmware_model_compute_readiness(void);
bool firmware_model_detect_fever(void);
bool firmware_model_estimate_stress(void);
float firmware_model_get_temp_ema(void);

#endif
