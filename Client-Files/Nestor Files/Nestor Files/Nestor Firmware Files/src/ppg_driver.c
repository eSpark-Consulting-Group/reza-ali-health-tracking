
#include "ppg_driver.h"
#include "max86178.h"
#include "nrf_log.h"

static uint8_t current_hr = 0;
static uint8_t current_spo2 = 0;

void ppg_driver_init(void) {
    max86178_init(NULL);  // Assume I2C already initialized elsewhere
    NRF_LOG_INFO("PPG driver initialized.");
}

void ppg_driver_update(void) {
    max86178_read_ppg(&current_hr, &current_spo2);
}

uint8_t ppg_driver_get_hr(void) {
    return current_hr;
}

uint8_t ppg_driver_get_spo2(void) {
    return current_spo2;
}
