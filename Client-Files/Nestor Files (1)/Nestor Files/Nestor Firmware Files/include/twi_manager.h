#ifndef TWI_MANAGER_H
#define TWI_MANAGER_H

#include "nrf_drv_twi.h"

void twi_manager_init(void);
const nrf_drv_twi_t* twi_manager_get(void);

#endif
