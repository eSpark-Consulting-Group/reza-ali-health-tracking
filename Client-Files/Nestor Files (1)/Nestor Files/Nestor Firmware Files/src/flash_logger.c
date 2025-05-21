#include "flash_logger.h"
#include <string.h>

#define MAX_LOGS 256
static log_entry_t logs[MAX_LOGS];
static uint16_t log_index = 0;

void flash_logger_init(void) {
    memset(logs, 0, sizeof(logs));
    log_index = 0;
}

bool flash_logger_add(log_entry_t entry) {
    if (log_index >= MAX_LOGS) return false;
    logs[log_index++] = entry;
    return true;
}

void flash_logger_flush_to_ble(void) {
    // Placeholder: BLE send logic
    log_index = 0;
}
