
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

uint8_t readpins(void);
uint8_t readpins_debounced(void);

volatile uint8_t btnstate;

