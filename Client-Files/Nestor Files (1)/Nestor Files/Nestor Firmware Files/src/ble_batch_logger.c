#include "ble_batch_logger.h"
#include "ble_nestor_service.h"
#include "nrf_log.h"
#include <string.h>

#define BATCH_SIZE 10

typedef struct {
    uint8_t hr;
    uint8_t spo2;
    uint16_t temp;
    uint8_t batt;
    uint8_t motion;
    uint8_t readiness;
    bool fever;
} log_entry_t;

static log_entry_t log_buffer[BATCH_SIZE];
static uint8_t log_index = 0;

void ble_batch_logger_log(uint8_t hr, uint8_t spo2, uint16_t temp, uint8_t batt, uint8_t motion, uint8_t readiness, bool fever) {
    log_buffer[log_index++] = (log_entry_t){ hr, spo2, temp, batt, motion, readiness, fever };
    if (log_index >= BATCH_SIZE) {
        log_index = 0;
        NRF_LOG_INFO("[BLE BATCH] Uploading %d entries", BATCH_SIZE);
        for (int i = 0; i < BATCH_SIZE; ++i) {
            ble_nestor_service_update(log_buffer[i].hr, log_buffer[i].spo2, log_buffer[i].temp, log_buffer[i].batt, log_buffer[i].motion);
            ble_nestor_notify_readiness(log_buffer[i].readiness);
            ble_nestor_notify_fever_alert(log_buffer[i].fever);
        }
    }
}
