#include "nrf_log.h"
#include <stdint.h>

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
    NRF_LOG_ERROR("APP ERROR: %d at %s:%lu", error_code, p_file_name, line_num);
    NRF_LOG_FINAL_FLUSH();  // Ensure logs are output
    while (1);              // Halt system for debug
}
