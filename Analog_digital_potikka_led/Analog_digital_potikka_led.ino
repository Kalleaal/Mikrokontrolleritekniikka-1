#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "Analog.h"

// --------------------------------------------------
// UART 9600 baud
// --------------------------------------------------
void UART_init(void) {
    uint16_t ubrr = 103;  // 16 MHz → 9600 baud
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0B = (1 << TXEN0);                    // enable TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   // 8N1
}

void UART_sendChar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));  // wait for buffer
    UDR0 = c;
}

void UART_sendString(const char* s) {
    while (*s) UART_sendChar(*s++);
}

void UART_sendNumber(uint16_t n) {
    char buf[10];
    itoa(n, buf, 10);
    UART_sendString(buf);
}

// --------------------------------------------------
// PWM on OC1A (Pin 9)
// --------------------------------------------------
void PWM_init(void) {
    DDRB |= (1 << PB1);  // PB1 = OC1A = Arduino pin 9

    // Fast PWM 8-bit, non-inverting, prescaler 64
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
}

void PWM_set(uint8_t duty) {
    OCR1A = duty;  // 0–255
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main(void) {
    UART_init();
    ADC_init();
    PWM_init();

    while (1) {
        uint16_t sensorValue = ADC_read(1);      // 0–1023
        uint8_t outputValue  = sensorValue >> 2; // 0–255

        PWM_set(outputValue);

        UART_sendString("sensor = ");
        UART_sendNumber(sensorValue);
        UART_sendString("\t output = ");
        UART_sendNumber(outputValue);
        UART_sendString("\r\n");

        _delay_ms(2);
    }
}
