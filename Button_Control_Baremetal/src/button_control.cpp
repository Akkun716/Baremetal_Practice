#include <Arduino.h>
// Register Macros
#define MYPORTD *( (volatile byte *) 0x2B)
#define MYDDRD *( (volatile byte *) 0x2A)
#define MYPIND *( (volatile byte *) 0x29)
// Digital Input Macros
#define L_BUTTON (1 << PIND2) // Pin 2 (PD2)
#define M_BUTTON (1 << PIND4) // Pin 4 (PD4)
#define R_BUTTON (1 << PIND7) // Pin 7 (PD7)
// Digital Output Macros
#define L_LED (1 << PIND3) // Pin 3 (PD3)
#define M_LED (1 << PIND5) // Pin 5 (PD5)
#define R_LED (1 << PIND6) // Pin 6 (PD6)

// Value Macros
#define FLASH_COUNT 2 // Number of flashes for flash_bits

// Variables
bool button_pressed = false; // "Button active" variable
byte current_led;
byte lights[3] = {L_LED, M_LED, R_LED};
byte lights_len = 3;
  
void flash_leds(volatile byte *, byte);
void clear_bits(volatile byte *);
void shift_bit(volatile byte *, byte);

void setup() {
  MYDDRD |= L_LED | M_LED | R_LED;
  MYPORTD |= L_BUTTON | M_BUTTON | R_BUTTON;
  // Turns on lights[0]
  current_led = 0;
  MYPORTD |= lights[0];
}

void loop() {
  // M_BUTTON functionality
  if(!(MYPIND & M_BUTTON)) {
    if(!button_pressed) {
      button_pressed = true;
      //Toggle flash LEDS and reset active light to lights[0]
      flash_leds(&MYPORTD, FLASH_COUNT);
      clear_bits(&MYPORTD);
      current_led = 0;
      MYPORTD |= lights[0];
    }
  }
  
  // L_BUTTON functionality
  else if(!(MYPIND & L_BUTTON)) {
    if(!button_pressed) {
      // Shift active LED to lights[current_led - 1] and cycles to the left
      button_pressed = true;
      shift_bit(&MYPORTD, 1);
    }
  }
  
  // R_BUTTON functionality
  else if(!(MYPIND & R_BUTTON)) {
    if(!button_pressed) {
      // Shift active LED to lights[current_led + 1] and cycles to the right
      button_pressed = true;
      shift_bit(&MYPORTD, 0);
    }
  }

  else {
    // Resets "button active" status
    button_pressed = false;
  }

  // Debounce by 10 milliseconds
  delay(10);
}

/// @brief Flashes the specific 
/// @param bits 
/// @param flash_count 
void flash_leds(volatile byte * bits, byte flash_count) {
  for(byte count = 0; count < flash_count * 2; count++) {
    for(byte i = 0; i < lights_len; i++) {
      if(*bits & lights[i]) {
        *bits &= ~lights[i];
      } else {
        *bits |= lights[i];
      }
    }
    delay(500);
  }
}

void clear_bits(volatile byte * bits) {
  for(byte i = 0; i < lights_len; i++) {
      *bits &= ~lights[i];
  }
}

void shift_bit(volatile byte * bits, byte shift_forward) {
  *bits &= ~lights[current_led];
  switch(shift_forward) {
    case 0:
      if(current_led == (lights_len - 1)) {
        current_led = 0;
        *bits |= lights[0];
      } else {
        *bits |= lights[++current_led];
      }
      break;
    case 1:
      if(current_led == 0) {
        current_led = lights_len - 1;
        *bits |= lights[lights_len - 1];
      } else {
        *bits |= lights[--current_led];
      }
      break; 
  }
  
}