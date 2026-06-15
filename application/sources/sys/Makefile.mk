CFLAGS	+= -I$(SRC_ROOT)/sys
CPPFLAGS += -I$(SRC_ROOT)/sys

VPATH += $(SRC_ROOT)/sys

SOURCES += $(SRC_ROOT)/sys/sys_dbg.c
SOURCES += $(SRC_ROOT)/sys/sys_boot.c
