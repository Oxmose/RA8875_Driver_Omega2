#ifndef __DM_PLATFORM_H_
#define __DM_PLATFORM_H_

/* Global includes */
#include <stdint.h>


// Clear bit, Set bit, High pulse, Low pulse, Boundary limits and Delay macros
#define sbi(reg, _bitmask) (*(reg) = 1)
#define cbi(reg, _bitmask) (*(reg) = 0)
#define pulse_high(reg, _bitmask) do { *(reg) = 1; *(reg) = 0; } while(0)
#define pulse_low(reg, _bitmask) do { *(reg) = 0; *(reg) = 1; } while(0)
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>(high)?(high):(amt)))
#define delay(ms) usleep(ms * 1000)

// On Arduino no extra delay is needed, but on faster platforms the simulated
// SPI bus may get a too high frequency so a delay here will lower it. This
// delay should ideally be configurable per platform but for now it will have
// to be ok to lower the frequency to 500KHz  

// TODO CHECK HERE OMEGA 580Mhz
#define slow_pulse_delay() do {\
  usleep(2);\
} while(0)

#define slow_pulse_high(reg, _bitmask) do {\
   *(reg) = 1;    \
   slow_pulse_delay(); \
   *(reg) = 0;    \
   slow_pulse_delay(); \
} while(0)
#define slow_pulse_low(reg, _bitmask) do {\
   *(reg) = 0;    \
   slow_pulse_delay(); \
   *(reg) = 1;    \
   slow_pulse_delay(); \
} while(0)

#define SPECIAL_D5  D5

#endif /* __DM_PLATFORM_H_ */
