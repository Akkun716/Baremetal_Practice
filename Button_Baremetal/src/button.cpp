#include <Arduino.h>
// Register Macros
#define MYPORTD *( (volatile byte *) 0x2B)
#define MYDDRD *( (volatile byte *) 0x2A)
#define MYPIND *( (volatile byte *) 0x29)
// Digital Input Macros
#define BUTTON (1 << PIND2)
// Digital Output Macros
#define LED (1 << PIND3)

void setup() {
    DDRD |= LED;
    // Pullup internal resistor to high
    PORTD |= BUTTON;
}

void loop() {
    // If button input is low (button pressed)...
    if(!(PIND & BUTTON)) {
        PORTD |= LED;
    } else {
        PORTD &= ~LED;
    }
}