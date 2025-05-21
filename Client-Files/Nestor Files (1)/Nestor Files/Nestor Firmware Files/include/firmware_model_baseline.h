#ifndef FIRMWARE_MODEL_BASELINE_H
#define FIRMWARE_MODEL_BASELINE_H

#include <stdint.h>

void firmware_model_init(void);
void firmware_model_add_rr_interval(uint16_t rr);
void firmware_model_record_temperature(float temp);

#endif
