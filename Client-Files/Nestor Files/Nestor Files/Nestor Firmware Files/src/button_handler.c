#include "app_button.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "haptic.h"

#define BUTTON_PIN 13
#define SHORT_PRESS_MS 100
#define LONG_PRESS_MS 1000

static uint32_t press_time = 0;

void button_handler(uint8_t pin_no, uint8_t button_action) {
    if (button_action == APP_BUTTON_PUSH) {
        press_time = app_timer_cnt_get();
    } else {
        uint32_t release_time = app_timer_cnt_get();
        uint32_t diff_ticks = release_time - press_time;
        uint32_t diff_ms = APP_TIMER_TICKS_TO_MS(diff_ticks);

        if (diff_ms >= LONG_PRESS_MS) {
            NRF_LOG_INFO("Long press → System reset");
            NVIC_SystemReset();
        } else if (diff_ms >= SHORT_PRESS_MS) {
            NRF_LOG_INFO("Short press → Force BLE sync");
            haptic_vibrate_ms(100);
            // trigger_ble_sync(); // implement externally
        }
    }
}
