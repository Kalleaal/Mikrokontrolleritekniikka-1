#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>

void ADC_init(void);
uint16_t ADC_read(int channel);

#endif
