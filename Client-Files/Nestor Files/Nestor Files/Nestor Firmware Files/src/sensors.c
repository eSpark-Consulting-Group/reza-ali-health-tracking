#include "sensors.h"
#include "nrf_log.h"
#include "ble_nestor_service.h"
#include "battery_monitor.h"
#include "ppg_driver.h"
#include "haptic.h"
#include "twi_manager.h"
#include "hrv_engine.h"
#include "health_monitor.h"
#include "firmware_model_baseline.h"
#include "flash_logger_timestamp.h"
#include "max86178_rr.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "lis2dw12.h"
#include "sensor_duty_controller.h"
#include "wear_state_manager.h"

static const nrf_drv_twi_t* m_i2c = NULL;

static uint8_t last_hr = 0;
static float last_temp = 36.5f;
static float last_motion_score = 0.0f;

void sensors_init(void) {
    m_i2c = twi_manager_get();
    max86178_init(m_i2c);
    max86178_rr_init(m_i2c);
    lis2dw12_init(m_i2c);
    haptic_init(m_i2c);
    firmware_model_init();
    sensor_duty_controller_init();
    wear_state_manager_init();
    NRF_LOG_INFO("Sensors initialized.");
}

void sensors_sample_all(void) {
    uint8_t hr = 0, spo2 = 0;
    uint16_t ir_dc = max86178_read_ir_dc();  // replace with actual IR read API if needed
    max86178_read_ppg(&hr, &spo2);

    uint16_t temp = 365;  // Simulated or raw temp
    uint8_t batt = battery_get_percent();
    uint8_t motion = lis2dw12_get_activity_level();

    wear_state_manager_update(motion, ir_dc);
    sensor_duty_controller_update(motion);

    if (!wear_state_manager_is_worn()) {
        NRF_LOG_INFO("Device not worn — skipping sample.");
        return;
    }

    last_hr = hr;
    last_temp = temp / 10.0f;
    last_motion_score = motion;

    uint16_t rr = 0;
    if (max86178_extract_rr(&rr)) {
        firmware_model_add_rr_interval(rr);
    }

    firmware_model_record_temperature(last_temp);
    health_monitor_record_temp(last_temp);
    health_monitor_record_activity((float)motion);
}

uint8_t sensors_get_last_hr(void) {
    return last_hr;
}

float sensors_get_last_temp(void) {
    return last_temp;
}

float sensors_get_last_activity(void) {
    return last_motion_score;
}
