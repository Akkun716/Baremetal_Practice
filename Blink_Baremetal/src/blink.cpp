#include <Arduino.h>
#define MYPORTB *( (volatile byte *) 0x25)
#define MYDDRB *( (volatile byte *) 0x24)

// Analog Macros
#define LED (1 << 5)


void setup() {
 MYDDRB |= LED;
}

void loop() {
 MYPORTB |= LED;   // Turn LED on
 delay(1000);      // Wait 2 seconds
 MYPORTB &= ~LED;  // Turn LED off
 delay(1000);      // Wait 2 seconds
}