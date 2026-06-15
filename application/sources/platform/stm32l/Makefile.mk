include $(SRC_ROOT)/platform/stm32l/Libraries/STM32L1xx_StdPeriph_Driver/Makefile.mk
include $(SRC_ROOT)/platform/stm32l/Libraries/CMSIS/Makefile.mk
include $(SRC_ROOT)/platform/stm32l/arduino/Makefile.mk

LDFILE = $(SRC_ROOT)/platform/stm32l/ak.ld

CFLAGS += -I$(SRC_ROOT)/platform/stm32l

CPPFLAGS += -I$(SRC_ROOT)/platform/stm32l

VPATH += $(SRC_ROOT)/platform/stm32l

# C source files
SOURCES += $(SRC_ROOT)/platform/stm32l/platform.c
SOURCES += $(SRC_ROOT)/platform/stm32l/system.c
SOURCES += $(SRC_ROOT)/platform/stm32l/sys_cfg.c
SOURCES += $(SRC_ROOT)/platform/stm32l/io_cfg.c
SOURCES += $(SRC_ROOT)/platform/stm32l/system_stm32l1xx.c

# C++ source files
SOURCES_CPP += $(SRC_ROOT)/platform/stm32l/mini_cpp.cpp

# ASM source files
SOURCES_ASM += $(SRC_ROOT)/platform/stm32l/sys_ctrl.s
