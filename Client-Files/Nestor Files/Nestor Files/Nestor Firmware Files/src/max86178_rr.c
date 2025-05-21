#include "max86178_rr.h"
#include "nrf_log.h"

#define MAX86178_ADDR 0x50
#define REG_FIFO_DATA 0x01
#define FIFO_SAMPLE_SIZE 6

static const nrf_drv_twi_t* m_i2c = NULL;

void max86178_rr_init(const nrf_drv_twi_t *i2c) {
    m_i2c = i2c;
    NRF_LOG_INFO("MAX86178 RR extractor initialized");
}

bool max86178_extract_rr(uint16_t *rr_out) {
    uint8_t fifo_data[FIFO_SAMPLE_SIZE];
    uint8_t reg = REG_FIFO_DATA;
    ret_code_t err;

    err = nrf_drv_twi_tx(m_i2c, MAX86178_ADDR, &reg, 1, true);
    if (err != NRF_SUCCESS) return false;

    err = nrf_drv_twi_rx(m_i2c, MAX86178_ADDR, fifo_data, FIFO_SAMPLE_SIZE);
    if (err != NRF_SUCCESS) return false;

    // Simulated RR calculation — placeholder
    uint16_t ppg_val = ((fifo_data[0] << 16) | (fifo_data[1] << 8) | fifo_data[2]) >> 8;
    *rr_out = 800 + (ppg_val % 50);  // 800–850 ms

    return true;
}
