#include <Arduino.h>

#define BUTTON (1 << PIND2)
#define LED (1 << PIND3)

void setup() {
    DDRD |= LED;
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