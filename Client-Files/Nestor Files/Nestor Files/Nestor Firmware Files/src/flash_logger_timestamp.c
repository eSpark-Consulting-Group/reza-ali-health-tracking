#include "flash_logger_timestamp.h"
#include "nrf_rtc.h"

uint32_t flash_logger_get_timestamp(void) {
    return NRF_RTC1->COUNTER;
}
