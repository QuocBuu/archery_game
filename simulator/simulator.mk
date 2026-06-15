SRC_ROOT           := ../application/sources
NAME_MODULE        ?= game_sim
APP_START_ADDR_VAL ?= 0x08003000

SIM_DEFINES += -DSSD1309_DRIVER_EN

# Stub headers shadow STM32/platform headers — MUST be first in -I order
CFLAGS   += -Iinc
CPPFLAGS += -Iinc

##############################################################################
# Pull in shared source lists and include paths from application
##############################################################################

include $(SRC_ROOT)/ak/Makefile.mk
include $(SRC_ROOT)/app/Makefile.mk
include $(SRC_ROOT)/common/Makefile.mk
include $(SRC_ROOT)/driver/Makefile.mk

# sys: header access only — sys_dbg.c / sys_boot.c pull STM32-specific sys_cfg.h
CFLAGS   += -I$(SRC_ROOT)/sys
CPPFLAGS += -I$(SRC_ROOT)/sys

# Arduino shims: only the subset the simulator needs (NOT platform Makefile.mk)
CPPFLAGS    += -I$(SRC_ROOT)/platform/stm32l/arduino/cores
CPPFLAGS    += -I$(SRC_ROOT)/platform/stm32l/arduino/cores/stm32
SOURCES     += $(SRC_ROOT)/platform/stm32l/arduino/cores/stm32/dtostrf.c
SOURCES_CPP += $(SRC_ROOT)/platform/stm32l/arduino/cores/Print.cpp
SOURCES_CPP += $(SRC_ROOT)/platform/stm32l/arduino/cores/WString.cpp
SOURCES_CPP += $(SRC_ROOT)/platform/stm32l/arduino/cores/itoa.cpp

##############################################################################
# Remove firmware-only sources replaced by simulator stubs
##############################################################################

SOURCES     := $(filter-out \
    $(SRC_ROOT)/driver/buzzer/buzzer.c \
    $(SRC_ROOT)/driver/flash/flash.c \
    ,$(SOURCES))

SOURCES_CPP := $(filter-out \
    $(SRC_ROOT)/app/shell.cpp \
    $(SRC_ROOT)/app/task_shell.cpp \
    ,$(SOURCES_CPP))

##############################################################################
# Simulator stub sources
##############################################################################

SOURCES     += src/sim_oled.c \
               src/sim_input.c

SOURCES_CPP += src/sim_main.cpp \
               src/sim_platform.cpp \
               src/sim_arduino.cpp \
               src/sim_shell.cpp

##############################################################################
# Simulator defines
##############################################################################

SIM_DEFINES += \
    -DSIMULATOR=1 \
    -DSYS_PRINT_EN \
    -DAPP_PRINT_EN \
    -DSTM32L_PLATFORM \
    -DAPP_START_ADDR=$(APP_START_ADDR_VAL)

CFLAGS   += $(SIM_DEFINES)
CPPFLAGS += $(SIM_DEFINES)
