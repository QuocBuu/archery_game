include $(SRC_ROOT)/common/container/Makefile.mk
#include $(SRC_ROOT)/common/ctreemap/Makefile.mk

CFLAGS		+= -I$(SRC_ROOT)/common
CPPFLAGS	+= -I$(SRC_ROOT)/common

VPATH += $(SRC_ROOT)/common

SOURCES_CPP += $(SRC_ROOT)/common/view_item.cpp
SOURCES_CPP += $(SRC_ROOT)/common/view_render.cpp
SOURCES_CPP += $(SRC_ROOT)/common/screen_manager.cpp

SOURCES += $(SRC_ROOT)/common/utils.c
SOURCES += $(SRC_ROOT)/common/xprintf.c
SOURCES += $(SRC_ROOT)/common/cmd_line.c
