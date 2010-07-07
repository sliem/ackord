#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usb_keyboard.h"
#include "ackord.h"
#include "keymap.h"

int
main(void) {
  uint8_t t;

  CPU_PRESCALE(0); /* 16 MHz */

  /* configure the pins */
  DDRB  = 0x00;
  PORTB = 0x00;

  usb_init();

  /* wait for the OS */
  while (!usb_configured()) /* wait */ ;
  _delay_ms(1000);

  usb_keyboard_press(KEY_A, 0);
  
  /* timer0 to overflow every 256*1024 clock cycles */
  TCCR0A = 0x00;
  TCCR0B = 0x05;
  TIMSK0 = (1<<TOIE0);
  sei();
 
  
  while(1) {
    t = readpins_debounced();

    if (t != 0) {
      usb_keyboard_press(pgm_read_byte_near(keymap + t), 0);
    }
  }
}

uint8_t
readpins(void) {
  return PINB;
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
  static volatile uint8_t oldstate, newstate, count;

  newstate = readpins();

  if (newstate == oldstate) { 
    count++;
  } else {
    count = 0;
  }

  oldstate = newstate;

  if (count > 10) {
    btnstate = newstate;
    count = 0;
  }
}
