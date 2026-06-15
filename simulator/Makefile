##############################################################################
# AK Base Kit - Arduino Base Simulator
#
# @Author : QuocBuu
# @Date   : 2026/06/16
#
# Base simulation framework for embedded software development and testing.
##############################################################################

Print = @echo "~"

PROJECT = $(NAME_MODULE)

# Build output directories
OBJECTS_DIR     = build_simulator
WEB_OBJECTS_DIR = build_simulator_web/obj
WEB_DIST_DIR    = build_simulator_web/dist

TARGET     = $(OBJECTS_DIR)/$(PROJECT)
WEB_TARGET = $(WEB_DIST_DIR)/$(PROJECT).html

# Native compilers
CC  = gcc
CPP = g++

# Web compilers
WEB_CC  = emcc
WEB_CPP = em++

include simulator.mk

##############################################################################
# Compiler flags
##############################################################################

SDL_CFLAGS := $(shell pkg-config --cflags sdl2 2>/dev/null)
SDL_LIBS   := $(shell pkg-config --libs   sdl2 2>/dev/null)

WARNING_FLAGS += \
    -Wall \
    -Wextra \
    -Wno-unused-parameter \
    -Wno-missing-field-initializers

CFLAGS   += -g -std=c99   $(WARNING_FLAGS) $(SDL_CFLAGS) -MMD -MP
CPPFLAGS += -g -std=c++11 $(WARNING_FLAGS) $(SDL_CFLAGS) -MMD -MP

LDFLAGS += $(SDL_LIBS)

WEB_CFLAGS   = -g -std=c99   $(WARNING_FLAGS) $(SIM_DEFINES) $(filter-out $(SDL_CFLAGS),$(CFLAGS))   -sUSE_SDL=2 -MMD -MP
WEB_CPPFLAGS = -g -std=c++11 $(WARNING_FLAGS) $(SIM_DEFINES) $(filter-out $(SDL_CFLAGS),$(CPPFLAGS)) -sUSE_SDL=2 -MMD -MP

WEB_LDFLAGS = \
    -sUSE_SDL=2 \
    -sALLOW_MEMORY_GROWTH=1 \
    -sASYNCIFY=1 \
    -sEXIT_RUNTIME=0 \
    -sNO_DISABLE_EXCEPTION_CATCHING=1 \
    "-sEXPORTED_RUNTIME_METHODS=['ccall']" \
    --shell-file=web/shell.html

##############################################################################
# Object lists
##############################################################################

APP_C_SOURCES   = $(filter $(SRC_ROOT)/%.c,   $(SOURCES))
SIM_C_SOURCES   = $(filter-out $(SRC_ROOT)/%.c,   $(SOURCES))
APP_CPP_SOURCES = $(filter $(SRC_ROOT)/%.cpp, $(SOURCES_CPP))
SIM_CPP_SOURCES = $(filter-out $(SRC_ROOT)/%.cpp, $(SOURCES_CPP))

# Native objects
OBJECTS += $(patsubst $(SRC_ROOT)/%.c,   $(OBJECTS_DIR)/sources/%.c.o,   $(APP_C_SOURCES))
OBJECTS += $(patsubst %.c,               $(OBJECTS_DIR)/%.c.o,            $(SIM_C_SOURCES))
OBJECTS += $(patsubst $(SRC_ROOT)/%.cpp, $(OBJECTS_DIR)/sources/%.cpp.o,  $(APP_CPP_SOURCES))
OBJECTS += $(patsubst %.cpp,             $(OBJECTS_DIR)/%.cpp.o,           $(SIM_CPP_SOURCES))

# Web objects
WEB_OBJECTS += $(patsubst $(SRC_ROOT)/%.c,   $(WEB_OBJECTS_DIR)/sources/%.c.o,   $(APP_C_SOURCES))
WEB_OBJECTS += $(patsubst %.c,               $(WEB_OBJECTS_DIR)/%.c.o,            $(SIM_C_SOURCES))
WEB_OBJECTS += $(patsubst $(SRC_ROOT)/%.cpp, $(WEB_OBJECTS_DIR)/sources/%.cpp.o,  $(APP_CPP_SOURCES))
WEB_OBJECTS += $(patsubst %.cpp,             $(WEB_OBJECTS_DIR)/%.cpp.o,           $(SIM_CPP_SOURCES))

DEPS     = $(OBJECTS:.o=.d)
WEB_DEPS = $(WEB_OBJECTS:.o=.d)

##############################################################################
# Native targets
##############################################################################

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

##############################################################################
# Web targets
##############################################################################

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

.PHONY: web-deploy
web-deploy: web
	$(Print) DEPLOY to ../docs/simulator/
	@mkdir -p ../docs/simulator
	@cp $(WEB_DIST_DIR)/* ../docs/simulator/
	@touch ../docs/simulator/.nojekyll
	$(Print) Done — commit and push docs/simulator/ to publish on GitHub Pages

##############################################################################
# Utilities
##############################################################################

.PHONY: init
init:
	$(Print) Checking build dependencies...
	@command -v gcc    >/dev/null 2>&1 || { echo "ERROR: gcc not found";         exit 1; }
	@command -v g++    >/dev/null 2>&1 || { echo "ERROR: g++ not found";         exit 1; }
	@pkg-config --exists sdl2         || { echo "ERROR: libsdl2-dev not found — run: sudo apt install libsdl2-dev"; exit 1; }
	@command -v emcc   >/dev/null 2>&1 || { echo "WARNING: emcc not found — web build unavailable (install emsdk)"; }
	$(Print) All required dependencies are present.

.PHONY: clean
clean:
	$(Print) CLEAN $(OBJECTS_DIR)
	@rm -rf $(OBJECTS_DIR)

.PHONY: clean-web
clean-web:
	$(Print) CLEAN $(WEB_OBJECTS_DIR) $(WEB_DIST_DIR)
	@rm -rf build_simulator_web

.PHONY: clean-all
clean-all: clean clean-web

.PHONY: help
help:
	$(Print) "[make]            build native Linux simulator"
	$(Print) "[make run]        build and run native Linux simulator on X11"
	$(Print) "[make web]        build browser simulator with Emscripten"
	$(Print) "[make web-serve]  build and serve browser simulator locally"
	$(Print) "[make web-deploy] build and copy to ../docs/simulator/ for GitHub Pages"
	$(Print) "[make init]       check build dependencies"
	$(Print) "[make clean]      clean native build"
	$(Print) "[make clean-web]  clean web build"
	$(Print) "[make clean-all]  clean all builds"

-include $(DEPS)
-include $(WEB_DEPS)
