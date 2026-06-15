#stm32 libs path
CPPFLAGS	+= -I$(SRC_ROOT)/platform/stm32l/Libraries/STM32L1xx_StdPeriph_Driver/inc
CPPFLAGS	+= -I$(SRC_ROOT)/platform/stm32l/Libraries/CMSIS/Device/ST/STM32L1xx/Include
CPPFLAGS	+= -I$(SRC_ROOT)/platform/stm32l/Libraries/CMSIS/Include

CFLAGS		+= -I$(SRC_ROOT)/platform/stm32l/arduino/cores/stm32
CPPFLAGS	+= -I$(SRC_ROOT)/platform/stm32l/arduino/cores/stm32

VPATH += $(SRC_ROOT)/platform/stm32l/arduino/cores/stm32

SOURCES += $(SRC_ROOT)/platform/stm32l/arduino/cores/stm32/dtostrf.c
SOURCES += $(SRC_ROOT)/platform/stm32l/arduino/cores/stm32/hooks.c
