
// Nestor Firmware: Master Version
// Includes: Sensor pipeline, SQI filtering, BLE Secure Notify, Power Management, UX, Logging

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_clock.h"
#include "app_timer.h"
#include "app_button.h"

#include "ble_stack_init.h"
#include "twi_manager.h"
#include "sensors.h"
#include "ble_nestor_service.h"
#include "sensor_duty_controller.h"
#include "hrv_engine.h"
#include "health_monitor.h"
#include "calorie_estimation.h"
#include "wear_state_manager.h"
#include "flash_logger_timestamp.h"
#include "watchdog.h"

#include "sqi_filter.h"
#include "haptic.h"
#include "button_handler.h"
#include "ppg_driver.h"
#include "ble_batch_logger.h"

#define SENSOR_SAMPLE_INTERVAL APP_TIMER_TICKS(5000)
#define HEALTH_UPDATE_INTERVAL APP_TIMER_TICKS(30000)
#define BATCH_SYNC_INTERVAL APP_TIMER_TICKS(60000)

APP_TIMER_DEF(sensor_timer_id);
APP_TIMER_DEF(health_timer_id);
APP_TIMER_DEF(batch_timer_id);

static float daily_calories = 0.0f;

static void sensor_timeout_handler(void *p_context) {
    if (sensor_duty_controller_sampling_enabled()) {
        sensors_sample_all();
    }
}

static void health_timeout_handler(void *p_context) {
    float hrv_rmssd = hrv_engine_compute_rmssd();
    float temp_z = health_monitor_compute_temp_zscore(sensors_get_last_temp());
    uint8_t hr = sensors_get_last_hr();
    float recent_act = sensors_get_last_activity();

    float sqi = sqi_filter_process(sensors_get_ppg_signal(), sensors_get_ppg_len());
    if (recent_act > 50 || sqi < 0.6f) {
        NRF_LOG_WARNING("Signal or motion issue: HR ignored");
        return;
    }

    uint8_t readiness = health_monitor_compute_readiness(hr, hrv_rmssd, temp_z, recent_act);
    uint8_t stress = health_monitor_estimate_stress(hr, hrv_rmssd);
    bool fever = health_monitor_check_fever_alert(temp_z, hrv_rmssd);

    float kcal = estimate_calories_hr_based(hr, 70.0f, 30, true, 0.5f);
    daily_calories += kcal;

    ble_nestor_update_readiness(readiness);
    ble_nestor_update_stress(stress);
    ble_nestor_update_fever_alert(fever ? 1 : 0);
    ble_nestor_update_calories(daily_calories);

    log_entry_t log = {
        .timestamp = flash_logger_get_timestamp(),
        .hr = hr,
        .rr = (uint16_t)(60000.0f / (hr + 1)),
        .temp = sensors_get_last_temp(),
        .motion = recent_act
    };
    flash_logger_add(log);

    ble_batch_logger_log(hr, ppg_driver_get_spo2(), sensors_get_last_temp() * 100, 90, recent_act, readiness, fever);

    NRF_LOG_INFO("R=%d S=%d F=%d C=%.1f", readiness, stress, fever, daily_calories);
}

static void batch_sync_handler(void *p_context) {
    ble_batch_logger_flush();
}

static void timers_init(void) {
    app_timer_init();
    app_timer_create(&sensor_timer_id, APP_TIMER_MODE_REPEATED, sensor_timeout_handler);
    app_timer_create(&health_timer_id, APP_TIMER_MODE_REPEATED, health_timeout_handler);
    app_timer_create(&batch_timer_id, APP_TIMER_MODE_REPEATED, batch_sync_handler);
}

static void buttons_init(void) {
    static app_button_cfg_t button_cfg = {
        .pin_no = 13,
        .active_state = APP_BUTTON_ACTIVE_LOW,
        .pull_cfg = NRF_GPIO_PIN_PULLUP,
        .button_handler = button_handler
    };
    app_button_init(&button_cfg, 1, APP_TIMER_TICKS(50));
    app_button_enable();
}

static void log_init(void) {
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void power_manage(void) {
    nrf_pwr_mgmt_run();
}

int main(void) {
    log_init();
    nrf_drv_clock_init();
    nrf_drv_clock_lfclk_request(NULL);
    nrf_pwr_mgmt_init();
    timers_init();
    buttons_init();
    twi_manager_init();
    ble_stack_init();
    ble_nestor_service_init();
    haptic_init();
    watchdog_init();

    sensors_init();
    ppg_driver_init();
    sensor_duty_controller_init();

    wear_state_manager_update(0, 0);
    NRF_LOG_INFO("[BOOT] Nestor firmware full stack init complete");

    app_timer_start(sensor_timer_id, SENSOR_SAMPLE_INTERVAL, NULL);
    app_timer_start(health_timer_id, HEALTH_UPDATE_INTERVAL, NULL);
    app_timer_start(batch_timer_id, BATCH_SYNC_INTERVAL, NULL);

    while (true) {
        if (!NRF_LOG_PROCESS()) {
            power_manage();
        }
    }
}
