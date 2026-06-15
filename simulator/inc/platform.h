#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>

#define ENABLE_INTERRUPTS()  do {} while (0)
#define DISABLE_INTERRUPTS() do {} while (0)
#define ENTRY_CRITICAL()     do {} while (0)
#define EXIT_CRITICAL()      do {} while (0)
#define LOG2LKUP(val)        ((val) ? (uint_fast8_t)(32U - __builtin_clz((uint32_t)(val))) : 0U)

#endif
