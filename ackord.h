
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define MODIFIER_OFFSET 100

uint8_t readpins_debounced(void);
void    init(void);

volatile uint8_t btnstate;

