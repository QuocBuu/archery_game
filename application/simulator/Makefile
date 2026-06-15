##############################################################################
# @brief: Native Linux simulator build for AK application source farm
##############################################################################

Print = @echo "~"

APP_ROOT = ..
SRC_ROOT = $(APP_ROOT)/sources

NAME_MODULE ?= game_sim
PROJECT = $(NAME_MODULE)

# Native build output
OBJECTS_DIR = build_make
TARGET      = $(OBJECTS_DIR)/$(PROJECT)

# Web build output
WEB_OBJECTS_DIR = web/build/obj
WEB_DIST_DIR    = web/build/dist
WEB_TARGET      = $(WEB_DIST_DIR)/$(PROJECT).html

# Compilers
CC      = gcc
CPP     = g++
WEB_CC  = emcc
WEB_CPP = em++

APP_START_ADDR_VAL ?= 0x08003000

SIM_DEFINES += \
	-DSIMULATOR=1 \
	-DSYS_PRINT_EN \
	-DAPP_PRINT_EN \
	-DSTM32L_PLATFORM \
	-DAPP_START_ADDR=$(APP_START_ADDR_VAL)

SIM_SOURCE_ROOTS += \
	$(SRC_ROOT)/ak/src \
	$(SRC_ROOT)/app \
	$(SRC_ROOT)/common \
	$(SRC_ROOT)/driver

SIM_CORE_SOURCES += \
	src/sim_main.cpp \
	src/sim_platform.cpp \
	src/sim_arduino.cpp \
	src/sim_oled.c \
	src/sim_input.c \
	src/sim_shell.cpp

SIM_EXTRA_SOURCES += \
	$(SRC_ROOT)/platform/stm32l/arduino/cores/Print.cpp \
	$(SRC_ROOT)/platform/stm32l/arduino/cores/WString.cpp \
	$(SRC_ROOT)/platform/stm32l/arduino/cores/itoa.cpp \
	$(SRC_ROOT)/platform/stm32l/arduino/cores/stm32/dtostrf.c

SIM_EXCLUDE_SOURCES += \
	$(SRC_ROOT)/app/shell.cpp \
	$(SRC_ROOT)/app/task_shell.cpp \
	$(SRC_ROOT)/driver/buzzer/buzzer.c \
	$(SRC_ROOT)/driver/flash/flash.c

SIM_EXCLUDE_DIRS += \
	$(SRC_ROOT)/driver/AsyncDelay

SIM_INCLUDE_DIRS += \
	include \
	$(SRC_ROOT)/ak/inc \
	$(SRC_ROOT)/app \
	$(SRC_ROOT)/app/game \
	$(SRC_ROOT)/app/screens \
	$(SRC_ROOT)/common \
	$(SRC_ROOT)/common/container \
	$(SRC_ROOT)/driver/Adafruit_oled_drv \
	$(SRC_ROOT)/driver/AsyncDelay/src \
	$(SRC_ROOT)/driver/button \
	$(SRC_ROOT)/driver/buzzer \
	$(SRC_ROOT)/driver/eeprom \
	$(SRC_ROOT)/driver/flash \
	$(SRC_ROOT)/driver/gpio \
	$(SRC_ROOT)/driver/led \
	$(SRC_ROOT)/platform/stm32l/arduino/cores \
	$(SRC_ROOT)/platform/stm32l/arduino/cores/stm32 \
	$(SRC_ROOT)/sys

-include project.mk

SDL_CFLAGS := $(shell pkg-config --cflags sdl2 2>/dev/null)
SDL_LIBS   := $(shell pkg-config --libs sdl2 2>/dev/null)

AUTO_INCLUDE_DIRS    = $(sort $(shell find $(SIM_SOURCE_ROOTS) -type d 2>/dev/null))
BASE_INCLUDE_FLAGS   = $(addprefix -I,$(SIM_INCLUDE_DIRS) $(AUTO_INCLUDE_DIRS))

WARNING_FLAGS += \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers

CFLAGS += \
	-g \
	-std=c99 \
	$(WARNING_FLAGS) \
	$(SIM_DEFINES) \
	$(BASE_INCLUDE_FLAGS) \
	$(SDL_CFLAGS) \
	-MMD \
	-MP

CPPFLAGS += \
	-g \
	-std=c++11 \
	$(WARNING_FLAGS) \
	$(SIM_DEFINES) \
	$(BASE_INCLUDE_FLAGS) \
	$(SDL_CFLAGS) \
	-MMD \
	-MP

LDFLAGS += $(SDL_LIBS)

WEB_CFLAGS = \
	-g \
	-std=c99 \
	$(WARNING_FLAGS) \
	$(SIM_DEFINES) \
	$(BASE_INCLUDE_FLAGS) \
	-sUSE_SDL=2 \
	-MMD \
	-MP

WEB_CPPFLAGS = \
	-g \
	-std=c++11 \
	$(WARNING_FLAGS) \
	$(SIM_DEFINES) \
	$(BASE_INCLUDE_FLAGS) \
	-sUSE_SDL=2 \
	-MMD \
	-MP

WEB_LDFLAGS = \
	-sUSE_SDL=2 \
	-sALLOW_MEMORY_GROWTH=1 \
	-sASYNCIFY=1 \
	-sEXIT_RUNTIME=0 \
	-sNO_DISABLE_EXCEPTION_CATCHING=1 \
	"-sEXPORTED_RUNTIME_METHODS=['ccall']" \
	--shell-file=web/shell.html

SOURCES     += $(shell find $(SIM_SOURCE_ROOTS) -type f -name '*.c'   2>/dev/null)
SOURCES_CPP += $(shell find $(SIM_SOURCE_ROOTS) -type f -name '*.cpp' 2>/dev/null)

SOURCES     += $(filter %.c,  $(SIM_CORE_SOURCES) $(SIM_EXTRA_SOURCES))
SOURCES_CPP += $(filter %.cpp,$(SIM_CORE_SOURCES) $(SIM_EXTRA_SOURCES))

SOURCES     := $(sort $(filter-out $(SIM_EXCLUDE_SOURCES),$(SOURCES)))
SOURCES_CPP := $(sort $(filter-out $(SIM_EXCLUDE_SOURCES),$(SOURCES_CPP)))

define filter_exclude_dirs
$(foreach dir,$(SIM_EXCLUDE_DIRS),$(filter-out $(dir)/%,$(1)))
endef

SOURCES     := $(call filter_exclude_dirs,$(SOURCES))
SOURCES_CPP := $(call filter_exclude_dirs,$(SOURCES_CPP))
SOURCES     := $(sort $(SOURCES))
SOURCES_CPP := $(sort $(SOURCES_CPP))

APP_C_SOURCES   = $(filter $(SRC_ROOT)/%.c,  $(SOURCES))
SIM_C_SOURCES   = $(filter-out $(SRC_ROOT)/%.c,  $(SOURCES))
APP_CPP_SOURCES = $(filter $(SRC_ROOT)/%.cpp,$(SOURCES_CPP))
SIM_CPP_SOURCES = $(filter-out $(SRC_ROOT)/%.cpp,$(SOURCES_CPP))

# Native objects
OBJECTS += $(patsubst $(SRC_ROOT)/%.c,  $(OBJECTS_DIR)/sources/%.c.o,  $(APP_C_SOURCES))
OBJECTS += $(patsubst %.c,              $(OBJECTS_DIR)/%.c.o,           $(SIM_C_SOURCES))
OBJECTS += $(patsubst $(SRC_ROOT)/%.cpp,$(OBJECTS_DIR)/sources/%.cpp.o,$(APP_CPP_SOURCES))
OBJECTS += $(patsubst %.cpp,            $(OBJECTS_DIR)/%.cpp.o,         $(SIM_CPP_SOURCES))

# Web objects
WEB_OBJECTS += $(patsubst $(SRC_ROOT)/%.c,  $(WEB_OBJECTS_DIR)/sources/%.c.o,  $(APP_C_SOURCES))
WEB_OBJECTS += $(patsubst %.c,              $(WEB_OBJECTS_DIR)/%.c.o,           $(SIM_C_SOURCES))
WEB_OBJECTS += $(patsubst $(SRC_ROOT)/%.cpp,$(WEB_OBJECTS_DIR)/sources/%.cpp.o,$(APP_CPP_SOURCES))
WEB_OBJECTS += $(patsubst %.cpp,            $(WEB_OBJECTS_DIR)/%.cpp.o,         $(SIM_CPP_SOURCES))

DEPS     = $(OBJECTS:.o=.d)
WEB_DEPS = $(WEB_OBJECTS:.o=.d)

# ---- Native targets --------------------------------------------------------

.PHONY: all
all: create $(TARGET)

.PHONY: create
create:
	$(Print) CREATE $(OBJECTS_DIR) folder
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS)
	$(Print) LD $@
	@$(CPP) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJECTS_DIR)/%.c.o: %.c
	$(Print) CC $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECTS_DIR)/%.cpp.o: %.cpp
	$(Print) CXX $@
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) -o $@ -c $<

$(OBJECTS_DIR)/sources/%.c.o: $(SRC_ROOT)/%.c
	$(Print) CC $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECTS_DIR)/sources/%.cpp.o: $(SRC_ROOT)/%.cpp
	$(Print) CXX $@
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) -o $@ -c $<

.PHONY: run
run: all
	@env DISPLAY=$${DISPLAY:-:0} SDL_VIDEODRIVER=x11 ./$(TARGET)

# ---- Web targets -----------------------------------------------------------

.PHONY: web
web: web-create $(WEB_TARGET)
	$(Print) COPY index.html
	@cp $(WEB_TARGET) $(WEB_DIST_DIR)/index.html

.PHONY: web-create
web-create:
	$(Print) CREATE $(WEB_DIST_DIR) folder
	@mkdir -p $(WEB_DIST_DIR)

$(WEB_TARGET): $(WEB_OBJECTS)
	$(Print) EMLD $@
	@$(WEB_CPP) -o $@ $(WEB_OBJECTS) $(WEB_LDFLAGS)

$(WEB_OBJECTS_DIR)/%.c.o: %.c
	$(Print) EMCC $@
	@mkdir -p $(dir $@)
	@$(WEB_CC) $(WEB_CFLAGS) -o $@ -c $<

$(WEB_OBJECTS_DIR)/%.cpp.o: %.cpp
	$(Print) EMCXX $@
	@mkdir -p $(dir $@)
	@$(WEB_CPP) $(WEB_CPPFLAGS) -o $@ -c $<

$(WEB_OBJECTS_DIR)/sources/%.c.o: $(SRC_ROOT)/%.c
	$(Print) EMCC $@
	@mkdir -p $(dir $@)
	@$(WEB_CC) $(WEB_CFLAGS) -o $@ -c $<

$(WEB_OBJECTS_DIR)/sources/%.cpp.o: $(SRC_ROOT)/%.cpp
	$(Print) EMCXX $@
	@mkdir -p $(dir $@)
	@$(WEB_CPP) $(WEB_CPPFLAGS) -o $@ -c $<

.PHONY: web-serve
web-serve: web
	$(Print) SERVE http://127.0.0.1:8000/
	@python3 -m http.server 8000 -d $(WEB_DIST_DIR)

# ---- Common targets --------------------------------------------------------

.PHONY: clean
clean:
	$(Print) CLEAN $(OBJECTS_DIR)
	@rm -rf $(OBJECTS_DIR)

.PHONY: clean-web
clean-web:
	$(Print) CLEAN web/build
	@rm -rf web/build

.PHONY: clean-all
clean-all: clean clean-web

.PHONY: help
help:
	$(Print) "[make]           build native Linux simulator"
	$(Print) "[make run]       build and run native Linux simulator on X11"
	$(Print) "[make web]       build browser simulator with Emscripten"
	$(Print) "[make web-serve] build and serve browser simulator locally"
	$(Print) "[make clean]     clean native build"
	$(Print) "[make clean-web] clean web build"
	$(Print) "[make clean-all] clean all builds"

-include $(DEPS)
-include $(WEB_DEPS)
