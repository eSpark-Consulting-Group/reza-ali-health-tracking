#ifndef BLE_NESTOR_SERVICE_H
#define BLE_NESTOR_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"

// Initializes the Nestor BLE service and all characteristics
void ble_nestor_service_init(void);

// Sends a readiness score (0–100) over BLE
void ble_nestor_update_readiness(uint8_t score);

// Sends a stress score (0–100) over BLE
void ble_nestor_update_stress(uint8_t score);

// Sends a fever alert (1 = alert, 0 = normal) over BLE
void ble_nestor_update_fever_alert(uint8_t status);

// Sends total daily calories burned (float) over BLE
void ble_nestor_update_calories(float kcal);

#endif // BLE_NESTOR_SERVICE_H
