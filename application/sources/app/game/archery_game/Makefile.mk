CFLAGS		+= -I$(SRC_ROOT)/app/game/archery_game
CPPFLAGS	+= -I$(SRC_ROOT)/app/game/archery_game

VPATH += $(SRC_ROOT)/app/game/archery_game

# CPP source files 
# Archery game 
SOURCES_CPP += $(SRC_ROOT)/app/game/archery_game/ar_game_archery.cpp
SOURCES_CPP += $(SRC_ROOT)/app/game/archery_game/ar_game_arrow.cpp
SOURCES_CPP += $(SRC_ROOT)/app/game/archery_game/ar_game_bang.cpp
SOURCES_CPP += $(SRC_ROOT)/app/game/archery_game/ar_game_border.cpp
SOURCES_CPP += $(SRC_ROOT)/app/game/archery_game/ar_game_meteoroid.cpp