#ifndef BLE_BATCH_LOGGER_H
#define BLE_BATCH_LOGGER_H

#include <stdint.h>
#include <stdbool.h>

void ble_batch_logger_log(
    uint8_t hr,
    uint8_t spo2,
    uint16_t temp,
    uint8_t batt,
    uint8_t motion,
    uint8_t readiness,
    bool fever
);

#endif
