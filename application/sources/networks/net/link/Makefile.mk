include $(SRC_ROOT)/networks/net/link/hal/Makefile.mk

CPPFLAGS	+= -I$(SRC_ROOT)/networks/net/link

VPATH += $(SRC_ROOT)/networks/net/link

SOURCES_CPP += $(SRC_ROOT)/networks/net/link/link.cpp
SOURCES_CPP += $(SRC_ROOT)/networks/net/link/link_mac.cpp
SOURCES_CPP += $(SRC_ROOT)/networks/net/link/link_phy.cpp
SOURCES_CPP += $(SRC_ROOT)/networks/net/link/link_data.cpp
