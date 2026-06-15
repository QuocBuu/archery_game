include $(SRC_ROOT)/ak/ak.cfg.mk

CFLAGS += -I$(SRC_ROOT)/ak/inc
CPPFLAGS += -I$(SRC_ROOT)/ak/inc

VPATH += $(SRC_ROOT)/ak/src

SOURCES += $(SRC_ROOT)/ak/src/fsm.c
SOURCES += $(SRC_ROOT)/ak/src/tsm.c
SOURCES += $(SRC_ROOT)/ak/src/task.c
SOURCES += $(SRC_ROOT)/ak/src/timer.c
SOURCES += $(SRC_ROOT)/ak/src/message.c
