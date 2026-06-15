include $(SRC_ROOT)/app/game/Makefile.mk
include $(SRC_ROOT)/app/screens/Makefile.mk

CFLAGS		+= -I$(SRC_ROOT)/app
CPPFLAGS	+= -I$(SRC_ROOT)/app

VPATH += $(SRC_ROOT)/app

# CPP source files
SOURCES_CPP += $(SRC_ROOT)/app/task_system.cpp
SOURCES_CPP += $(SRC_ROOT)/app/app_eeprom.cpp
SOURCES_CPP += $(SRC_ROOT)/app/app.cpp
SOURCES_CPP += $(SRC_ROOT)/app/app_data.cpp
SOURCES_CPP += $(SRC_ROOT)/app/app_non_clear_ram.cpp
SOURCES_CPP += $(SRC_ROOT)/app/app_bsp.cpp
SOURCES_CPP += $(SRC_ROOT)/app/shell.cpp

SOURCES_CPP += $(SRC_ROOT)/app/task_shell.cpp
SOURCES_CPP += $(SRC_ROOT)/app/task_life.cpp
SOURCES_CPP += $(SRC_ROOT)/app/task_fw.cpp
SOURCES_CPP += $(SRC_ROOT)/app/task_list.cpp
SOURCES_CPP += $(SRC_ROOT)/app/task_display.cpp

SOURCES_CPP += $(SRC_ROOT)/app/task_if.cpp
SOURCES_CPP += $(SRC_ROOT)/app/task_uart_if.cpp
