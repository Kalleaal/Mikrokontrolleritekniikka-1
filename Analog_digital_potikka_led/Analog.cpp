#include <avr/io.h>
#include "analog.h"

// Initialize ADC on ATmega328P (Arduino Uno)
void ADC_init(void) {
    ADMUX = (1 << REFS0);  // AVcc reference, ADC0
    ADCSRA = (1 << ADEN)   // enable ADC
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // prescaler 128
}

// Read 10-bit value from ADC0
uint16_t ADC_read(int channel) {
    ADMUX &= 0xF0;
    ADMUX |= channel;
    ADCSRA |= (1 << ADSC);         // start conversion
    while (ADCSRA & (1 << ADSC));  // wait until done
    return ADC;                    // return 10-bit result
}
