#include "lis2dw12.h"
#include "nrf_log.h"

#define LIS2DW12_ADDR        0x19
#define LIS2DW12_REG_CTRL1   0x20
#define LIS2DW12_REG_OUT_X_L 0x28

static const nrf_drv_twi_t* m_i2c = NULL;

void lis2dw12_init(const nrf_drv_twi_t *i2c) {
    m_i2c = i2c;
    uint8_t config[] = { LIS2DW12_REG_CTRL1, 0x47 }; // 100 Hz, Low Power Mode
    ret_code_t err = nrf_drv_twi_tx(m_i2c, LIS2DW12_ADDR, config, sizeof(config), false);
    APP_ERROR_CHECK(err);
    NRF_LOG_INFO("LIS2DW12 initialized.");
}

uint8_t lis2dw12_get_activity_level(void) {
    uint8_t reg = LIS2DW12_REG_OUT_X_L | 0x80;
    uint8_t data[6];
    if (nrf_drv_twi_tx(m_i2c, LIS2DW12_ADDR, &reg, 1, true) != NRF_SUCCESS) return 0;
    if (nrf_drv_twi_rx(m_i2c, LIS2DW12_ADDR, data, 6) != NRF_SUCCESS) return 0;

    int16_t x = (int16_t)(data[1] << 8 | data[0]);
    int16_t y = (int16_t)(data[3] << 8 | data[2]);
    int16_t z = (int16_t)(data[5] << 8 | data[4]);

    uint32_t mag = (x*x + y*y + z*z);
    if (mag > 100000) return 3;    // Active
    if (mag > 20000) return 2;     // Moderate
    if (mag > 2000) return 1;      // Low
    return 0;                      // Still
}
