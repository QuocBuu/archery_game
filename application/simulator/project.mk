##############################################################################
# Project-local simulator configuration.
#
# Keep application logic in ../sources/app. Edit this file only when a project
# needs a different display driver define, app start address, source root, or
# hardware-only exclusion.
##############################################################################

NAME_MODULE = game_sim
APP_START_ADDR_VAL = 0x08003000

SIM_DEFINES += \
	-DSSD1309_DRIVER_EN

# Example for another project in the same source farm:
# SIM_SOURCE_ROOTS += $(SRC_ROOT)/networks
# SIM_EXCLUDE_SOURCES += $(SRC_ROOT)/app/my_hardware_only_file.cpp

