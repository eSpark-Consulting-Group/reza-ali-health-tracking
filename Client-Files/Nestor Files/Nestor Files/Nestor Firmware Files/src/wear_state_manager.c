#include "wear_state_manager.h"
#include <stdbool.h>
#include <stdint.h>

static bool is_worn = false;

void wear_state_manager_update(uint8_t motion_level, uint16_t ir_dc_level) {
    is_worn = (motion_level > 10 || ir_dc_level > 10000);
}

bool wear_state_manager_is_worn(void) {
    return is_worn;
}
