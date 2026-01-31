#include <Arduino.h>
// Register Macros
#define MYPORTD *( (volatile byte *) 0x2B)
#define MYDDRD *( (volatile byte *) 0x2A)
#define MYPIND *( (volatile byte *) 0x29)
// Digital Input Macros
#define L_BUTTON (1 << PIND2) // Pin 2 (PD2)
#define M_BUTTON (1 << PIND3) // Pin 3 (PD3)
#define R_BUTTON (1 << PIND4) // Pin 4 (PD4)
// Digital Output Macros
#define SR_DATA (1 << PIND5) // Pin 5 (PD5)
#define SR_CLK (1 << PIND6) // Pin 6 (PD6)
#define LATCH_CLK (1 << PIND7) // Pin 7 (PD7)


// Value Macros
#define FLASH_COUNT 2 // Number of flashes for flash_bits
#define NUM_OF_DIGITS 10
#define LIGHTS_ON ~0b11111111
#define LIGHTS_OFF ~0b00000000

// Variables
bool button_pressed = false; // "Button active" variable
byte current_num;
byte digits[10] = { // Current setup: Common anode display
  (byte) ~0b00111111,  // 0
  (byte) ~0b00000110,  // 1
  (byte) ~0b01011011,  // 2
  (byte) ~0b01001111,  // 3
  (byte) ~0b01100110,  // 4
  (byte) ~0b01101101,  // 5
  (byte) ~0b01111101,  // 6
  (byte) ~0b00000111,  // 7
  (byte) ~0b01111111,  // 8
  (byte) ~0b01101111   // 9
};
  
void flash_lights();
void clear_bits();
void shift_byte(byte);
void shift_num(byte);

void setup() {
  MYDDRD |= SR_DATA | SR_CLK | LATCH_CLK;
  MYPORTD &= ~(SR_DATA | SR_CLK | LATCH_CLK);
  MYPORTD |= L_BUTTON | M_BUTTON | R_BUTTON;
  shift_byte(LIGHTS_OFF);
  // Turns on lights[0]
  current_num = 0;
  shift_byte(digits[0]);
}

void loop() {
  // M_BUTTON functionality
  if(!(MYPIND & M_BUTTON)) {
    if(!button_pressed) {
      button_pressed = true;
      flash_lights();
      current_num = 0;
      shift_byte(digits[0]);
    }
  }
  
  // L_BUTTON functionality
  else if(!(MYPIND & L_BUTTON)) {
    if(!button_pressed) {
      // Decrements number, cycle to '9' when bound reach
      button_pressed = true;
      shift_num(1);
    }
  }
  
  // R_BUTTON functionality
  else if(!(MYPIND & R_BUTTON)) {
    if(!button_pressed) {
      // Increments number, cycle to '0' when bound reached
      button_pressed = true;
      shift_num(0);
    }
  }

  else {
    // Resets "button active" status
    button_pressed = false;
  }

  // Debounce by 10 milliseconds
  delay(10);
}

// @brief Flashes the specific 
// @param bits 
// @param flash_count 
void flash_lights() {
  for(byte i = 0; i < FLASH_COUNT; i++) {
    shift_byte(LIGHTS_OFF);
    delay(500);
    shift_byte(digits[current_num]);
    delay(500);
  }
}

void shift_byte(byte data_in) {
  // Set latch to low, prevents premature output
  MYPORTD &= ~LATCH_CLK;
  // MSB serially copies data_in bits to shift register
  for(int i = 7; i >= 0; i--) {
    if(data_in & (1 << i)) {
      MYPORTD |= SR_DATA;
    } else {
      MYPORTD &= ~SR_DATA;
    }

    // Pulse shift clock to record next value
    MYPORTD |= SR_CLK;
    MYPORTD &= ~SR_CLK;
  }

  // Set latch to high, outputs saved shift register bits
  MYPORTD |= LATCH_CLK;
}

void shift_num(byte direction) {
  if(direction) {
    if(current_num == 0) {
      current_num = NUM_OF_DIGITS - 1;
    } else {
      current_num--;
    }
  } else {
    if(current_num == NUM_OF_DIGITS - 1) {
      current_num = 0;
    } else {
      current_num++;
    }
  }

  shift_byte(digits[current_num]);
}