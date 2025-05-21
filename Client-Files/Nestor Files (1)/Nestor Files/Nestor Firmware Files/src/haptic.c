#include "haptic.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"

#define HAPTIC_PIN 25

void haptic_init(void) {
    nrf_drv_gpiote_out_config_t config = GPIOTE_CONFIG_OUT_SIMPLE(false);
    nrf_drv_gpiote_out_init(HAPTIC_PIN, &config);
}

void haptic_vibrate_ms(uint16_t ms) {
    nrf_drv_gpiote_out_set(HAPTIC_PIN);
    nrf_delay_ms(ms);
    nrf_drv_gpiote_out_clear(HAPTIC_PIN);
}
