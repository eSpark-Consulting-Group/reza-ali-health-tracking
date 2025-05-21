#ifndef PPG_DRIVER_H
#define PPG_DRIVER_H

#include <stdint.h>
#include "nrf_drv_twi.h"

void max86178_init(const nrf_drv_twi_t *i2c);
void max86178_read_ppg(uint8_t *hr, uint8_t *spo2);

#endif
