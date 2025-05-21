#ifndef CALORIE_ESTIMATION_H
#define CALORIE_ESTIMATION_H

#include <stdint.h>
#include <stdbool.h>

float estimate_calories_hr_based(
    uint8_t hr,
    float weight_kg,
    uint8_t age,
    bool is_male,
    float duration_minutes
);

#endif
