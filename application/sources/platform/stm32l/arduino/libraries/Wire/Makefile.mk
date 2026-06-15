CPPFLAGS += -I$(SRC_ROOT)/platform/stm32l/arduino/libraries/Wire
CPPFLAGS += -I$(SRC_ROOT)/platform/stm32l/arduino/libraries/Wire/utility

VPATH += $(SRC_ROOT)/platform/stm32l/arduino/libraries/Wire
VPATH += $(SRC_ROOT)/platform/stm32l/arduino/libraries/Wire/utility

SOURCES_CPP += $(SRC_ROOT)/platform/stm32l/arduino/libraries/Wire/Wire.cpp
SOURCES_CPP += $(SRC_ROOT)/platform/stm32l/arduino/libraries/Wire/utility/twi.cpp
