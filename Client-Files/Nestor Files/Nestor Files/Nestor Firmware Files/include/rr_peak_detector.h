#ifndef RR_PEAK_DETECTOR_H
#define RR_PEAK_DETECTOR_H

#include <stdint.h>

void rr_peak_detector_reset(void);
uint16_t rr_peak_detector_extract(uint32_t *ppg_data, uint16_t length);

#endif
