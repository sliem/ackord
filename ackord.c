#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usb_keyboard.h"
#include "ackord.h"
#include "keymap.h"

int
main(void) {
  uint8_t oldkeystate, newkeystate, key;

  oldkeystate = newkeystate = key = 0;

  init();
  
  while(1) {
    newkeystate = readpins_debounced();

    if (newkeystate != oldkeystate) {
      key = pgm_read_byte_near(keymap + newkeystate);

      switch (key) {

      case KEY_CTRL  + MODIFIER_OFFSET: /* fallthrough */;
      case KEY_ALT   + MODIFIER_OFFSET: /* fallthrough */;
      case KEY_SHIFT + MODIFIER_OFFSET: /* fallthrough */;
      case KEY_GUI   + MODIFIER_OFFSET:
        keyboard_modifier_keys |= (key - MODIFIER_OFFSET);
	keyboard_keys[0] = KEY_K;
	usb_keyboard_send();
	keyboard_keys[0] = 0;
        break;

      default: 
	keyboard_keys[0] = key;
	usb_keyboard_send();
	keyboard_modifier_keys = keyboard_keys[0] = 0;
	break; 
      }
     }
    oldkeystate = newkeystate;
  }
}

void 
init(void) {
  CPU_PRESCALE(0); /* 16 MHz */

  /* configure the pins */
  DDRB  = 0x00;
  PORTB = 0xFF;

  usb_init();

  /* wait for the OS */
  while (!usb_configured()) /* wait */ ;
  _delay_ms(1000);

  /* timer0 to overflow every 256*1024 clock cycles */
  TCCR0A = 0x00;
  TCCR0B = 0x05;
  TIMSK0 = (1<<TOIE0);
  sei();
}

uint8_t
readpins_debounced(void) {
  uint8_t o, sreg;
  
  sreg = SREG;
  cli();
  o = btnstate;
  SREG = sreg;

  return o ^ 0xFF; /* 1 is true dammit */
}

ISR(TIMER0_OVF_vect) {
  static volatile uint8_t oldeststate, oldstate, newstate;

  newstate = PINB;

  if (newstate == oldstate &&
      newstate == oldeststate &&
      oldstate == oldeststate) { 
    btnstate = newstate;
  }

  oldeststate = oldstate;
  oldstate = newstate;
}
