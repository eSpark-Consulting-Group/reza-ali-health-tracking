#ifndef LIS2DW12_H
#define LIS2DW12_H

#include <stdint.h>
#include "nrf_drv_twi.h"

void lis2dw12_init(const nrf_drv_twi_t *i2c);
uint8_t lis2dw12_get_activity_level(void);

#endif
