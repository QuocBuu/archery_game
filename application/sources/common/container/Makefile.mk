CFLAGS		+= -I$(SRC_ROOT)/common/container
CPPFLAGS	+= -I$(SRC_ROOT)/common/container

VPATH += $(SRC_ROOT)/common/container

SOURCES += $(SRC_ROOT)/common/container/log_queue.c
SOURCES += $(SRC_ROOT)/common/container/fifo.c
SOURCES += $(SRC_ROOT)/common/container/ring_buffer.c
