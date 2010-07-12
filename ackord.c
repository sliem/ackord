#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usb_keyboard.h"
#include "ackord.h"
#include "keymap.h"

int
main(void) {
  static uint8_t oldkeystate, newkeystate;

  CPU_PRESCALE(0); /* 16 MHz */

  /* configure the pins */
  DDRB  = 0x00;
  PORTB = 0x00;

  usb_init();

  /* wait for the OS */
  while (!usb_configured()) /* wait */ ;
  _delay_ms(1000);

  /* timer0 to overflow every 256*1024 clock cycles */
  TCCR0A = 0x00;
  TCCR0B = 0x05;
  TIMSK0 = (1<<TOIE0);
  sei();
 
  
  while(1) {
    newkeystate = readpins_debounced();

    if (newkeystate != oldkeystate) {
      usb_keyboard_press(pgm_read_byte_near(keymap + newkeystate), 0);
    }
    oldkeystate = newkeystate;
  }
}

uint8_t
readpins_debounced(void) {
  uint8_t o, sreg;
  
  sreg = SREG;
  cli();
  o = btnstate;
  SREG = sreg;

  return o;
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
