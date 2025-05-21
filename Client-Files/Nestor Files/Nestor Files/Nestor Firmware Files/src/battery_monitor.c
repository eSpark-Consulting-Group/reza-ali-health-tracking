#include "battery_monitor.h"
#include "nrf_drv_saadc.h"
#include "nrf_log.h"
#include <math.h>

#define BATTERY_ADC_CHANNEL   0
#define ADC_REF_VOLTAGE       3.6f
#define ADC_RESOLUTION_BITS   10
#define VOLTAGE_DIVIDER_RATIO 2.0f
#define FILTER_ALPHA          0.05f
#define LOW_BATT_THRESHOLD    20

static float voltage_filtered = 3.8f;
static uint8_t last_percent = 100;

void battery_monitor_init(void) {
    ret_code_t err_code;

    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
    err_code = nrf_drv_saadc_init(NULL, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_saadc_channel_init(BATTERY_ADC_CHANNEL, &channel_config);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Battery monitor initialized.");
}

float battery_get_voltage(void) {
    nrf_saadc_value_t adc_val = 0;
    nrf_drv_saadc_sample_convert(BATTERY_ADC_CHANNEL, &adc_val);

    float adc_max = (1 << ADC_RESOLUTION_BITS);
    float voltage_raw = ((float)adc_val / adc_max) * ADC_REF_VOLTAGE * VOLTAGE_DIVIDER_RATIO;

    voltage_filtered = (FILTER_ALPHA * voltage_raw) + ((1.0f - FILTER_ALPHA) * voltage_filtered);
    return voltage_filtered;
}

uint8_t battery_get_percent(void) {
    float voltage = battery_get_voltage();
    uint8_t current = 100;

    if (voltage >= 4.2f) current = 100;
    else if (voltage >= 4.0f) current = 90;
    else if (voltage >= 3.9f) current = 80;
    else if (voltage >= 3.8f) current = 70;
    else if (voltage >= 3.7f) current = 60;
    else if (voltage >= 3.6f) current = 50;
    else if (voltage >= 3.5f) current = 40;
    else if (voltage >= 3.4f) current = 30;
    else if (voltage >= 3.3f) current = 20;
    else if (voltage >= 3.2f) current = 10;
    else current = 5;

    if (abs(current - last_percent) >= 2) {
        last_percent = current;
    }

    return last_percent;
}

void battery_check_warn(void) {
    if (battery_get_percent() <= LOW_BATT_THRESHOLD) {
        NRF_LOG_WARNING("Battery level is low: %d%%", last_percent);
        // Optional: trigger haptic or BLE alert
    }
}
