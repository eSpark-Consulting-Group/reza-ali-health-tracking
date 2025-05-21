#include "ble_nestor_service.h"
#include "ble_srv_common.h"
#include "nrf_log.h"
#include "app_error.h"

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;
static uint16_t m_service_handle;

static ble_gatts_char_handles_t hr_handles, spo2_handles, temp_handles;
static ble_gatts_char_handles_t batt_handles, motion_handles;
static ble_gatts_char_handles_t readiness_handles, fever_handles, stress_handles, calorie_handles;

static void add_char(uint16_t uuid, ble_gatts_char_handles_t *handles, uint8_t size) {
    ble_gatts_char_md_t char_md = { .char_props.read = 1, .char_props.notify = 1 };
    ble_gatts_attr_md_t cccd_md = { .vloc = BLE_GATTS_VLOC_STACK };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    char_md.p_cccd_md = &cccd_md;

    ble_uuid_t ble_uuid = { .uuid = uuid, .type = BLE_UUID_TYPE_BLE };
    ble_gatts_attr_md_t attr_md = { .vloc = BLE_GATTS_VLOC_STACK };
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    ble_gatts_attr_t attr_char_value = {
        .p_uuid = &ble_uuid,
        .p_attr_md = &attr_md,
        .init_len = size,
        .max_len = size,
        .init_offs = 0,
        .p_value = NULL
    };

    APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(m_service_handle, &char_md, &attr_char_value, handles));
}

void ble_nestor_service_init(void) {
    ble_uuid_t service_uuid = { .uuid = 0x180D, .type = BLE_UUID_TYPE_BLE };
    APP_ERROR_CHECK(sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &m_service_handle));

    add_char(0x2A37, &hr_handles, sizeof(uint8_t));
    add_char(0x2A5F, &spo2_handles, sizeof(uint8_t));
    add_char(0x2A1C, &temp_handles, sizeof(float));
    add_char(0x2A19, &batt_handles, sizeof(uint8_t));
    add_char(0x2A53, &motion_handles, sizeof(uint16_t));

    add_char(0xFFF0, &readiness_handles, sizeof(uint8_t));
    add_char(0xFFF1, &fever_handles, sizeof(uint8_t));
    add_char(0xFFF2, &stress_handles, sizeof(uint8_t));
    add_char(0xFFF3, &calorie_handles, sizeof(float));
}

void ble_nestor_update_char(ble_gatts_char_handles_t *handle, uint8_t *data, uint16_t len) {
    if (m_conn_handle == BLE_CONN_HANDLE_INVALID) return;

    ble_gatts_hvx_params_t params = {
        .handle = handle->value_handle,
        .type = BLE_GATT_HVX_NOTIFICATION,
        .p_data = data,
        .p_len = &len
    };
    sd_ble_gatts_hvx(m_conn_handle, &params);
}

void ble_nestor_update_readiness(uint8_t score) {
    ble_nestor_update_char(&readiness_handles, &score, sizeof(score));
}

void ble_nestor_update_stress(uint8_t score) {
    ble_nestor_update_char(&stress_handles, &score, sizeof(score));
}

void ble_nestor_update_fever_alert(uint8_t status) {
    ble_nestor_update_char(&fever_handles, &status, sizeof(status));
}

void ble_nestor_update_calories(float kcal) {
    ble_nestor_update_char(&calorie_handles, (uint8_t*)&kcal, sizeof(kcal));
}
