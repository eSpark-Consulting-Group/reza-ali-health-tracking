#include "ble_stack_init.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "app_timer.h"
#include "ble_conn_params.h"
#include "peer_manager.h"
#include "nrf_log.h"

void ble_stack_init(void) {
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(1, &ram_start);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    ble_gap_sec_params_t sec_params = {0};
    sec_params.bond = 1;
    sec_params.mitm = 1;
    sec_params.io_caps = BLE_GAP_IO_CAPS_DISPLAY_ONLY;
    sec_params.min_key_size = 7;
    sec_params.max_key_size = 16;

    pm_init();
    pm_sec_params_set(&sec_params);
    pm_register(NULL);

    NRF_LOG_INFO("BLE stack initialized with bonding + MITM protection");
}
