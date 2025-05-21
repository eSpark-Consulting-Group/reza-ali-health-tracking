#ifndef MAX86178_RR_H
#define MAX86178_RR_H

#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_twi.h"

void max86178_rr_init(const nrf_drv_twi_t *i2c);
bool max86178_extract_rr(uint16_t *rr_out);

#endif
