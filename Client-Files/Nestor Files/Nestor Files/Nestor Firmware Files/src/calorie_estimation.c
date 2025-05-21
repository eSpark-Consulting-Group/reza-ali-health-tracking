#include "calorie_estimation.h"

// Keytel HR-based formula (2005), includes weight, HR, age, sex
// Returns estimated kcal burned in `duration_minutes`

float estimate_calories_hr_based(
    uint8_t hr,
    float weight_kg,
    uint8_t age,
    bool is_male,
    float duration_minutes
) {
    float kcal_per_min = 0.0f;

    if (is_male) {
        kcal_per_min = (-55.0969f + 0.6309f * hr + 0.1988f * weight_kg + 0.2017f * age) / 4.184f;
    } else {
        kcal_per_min = (-20.4022f + 0.4472f * hr - 0.1263f * weight_kg + 0.074f * age) / 4.184f;
    }

    if (kcal_per_min < 0.0f) kcal_per_min = 0.0f;

    return kcal_per_min * duration_minutes;
}
