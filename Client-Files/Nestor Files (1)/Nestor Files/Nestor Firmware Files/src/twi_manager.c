#include "twi_manager.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"

#define TWI_INSTANCE_ID 0

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

void twi_manager_init(void) {
    nrf_drv_twi_config_t config = {
        .scl = 27,
        .sda = 26,
        .frequency = NRF_TWI_FREQ_400K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init = true  // Enable for reliability
    };

    APP_ERROR_CHECK(nrf_drv_twi_init(&m_twi, &config, NULL, NULL));
    nrf_drv_twi_enable(&m_twi);
    NRF_LOG_INFO("I2C initialized");
}

const nrf_drv_twi_t* twi_manager_get(void) {
    return &m_twi;
}
