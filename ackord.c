#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usb_keyboard.h"
#include "ackord.h"
#include "keymap.h"

int
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

uint8_t
readpins(void) {
  uint8_t b, d, o;

  b = PINB; d = PIND; o = 0;

  o |= b & (1<<0); /* B0 */
  o |= b & (1<<1); /* B1 */
  o |= b & (1<<2); /* B2 */
  o |= b & (1<<3); /* B3 */
  o |= b & (1<<4); /* B7 */

  if (d & (1<<0))  /* D0 */
    o |= (1<<5);

  if (d & (1<<1))  /* D1 */
    o |= (1<<6);

  if (d & (1<<2))  /* D2 */
    o |= (1<<7);

  return o;
}
