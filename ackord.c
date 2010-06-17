#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usb_keyboard.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

void
main(void) {
  CPU_PRESCALE(0); /* 16 MHz */

  usb_init();

  /* wait for the OS */
  while (!usb_configured()) /* wait */ ;
  _delay_ms(1000);

  usb_keyboard_press(KEY_A, 0);

  while(1) {
  }
}
