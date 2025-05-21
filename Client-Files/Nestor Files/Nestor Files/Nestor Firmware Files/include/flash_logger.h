#ifndef FLASH_LOGGER_H
#define FLASH_LOGGER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t timestamp;
    uint8_t hr;
    uint16_t rr;
    float temp;
    uint8_t motion;
} log_entry_t;

void flash_logger_init(void);
bool flash_logger_add(log_entry_t entry);
void flash_logger_flush_to_ble(void);

#endif
