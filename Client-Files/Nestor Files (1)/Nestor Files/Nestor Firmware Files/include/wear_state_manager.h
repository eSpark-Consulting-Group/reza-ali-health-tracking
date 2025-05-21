#ifndef WEAR_STATE_MANAGER_H
#define WEAR_STATE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

void wear_state_manager_update(uint8_t motion_level, uint16_t ir_dc_level);
bool wear_state_manager_is_worn(void);

#endif
