CPPFLAGS += -I$(SRC_ROOT)/driver/Adafruit_oled_drv

VPATH += $(SRC_ROOT)/driver/Adafruit_oled_drv

SOURCES_CPP += $(SRC_ROOT)/driver/Adafruit_oled_drv/Adafruit_GFX.cpp
SOURCES_CPP += $(SRC_ROOT)/driver/Adafruit_oled_drv/Adafruit_oled_drv.cpp
SOURCES_CPP += $(SRC_ROOT)/driver/Adafruit_oled_drv/glcdfont.cpp
