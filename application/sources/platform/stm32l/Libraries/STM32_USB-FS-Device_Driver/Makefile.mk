CFLAGS += -I$(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/inc
CPPFLAGS += -I$(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/inc

VPATH += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src

# C source files
SOURCES += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src/usb_core.c
SOURCES += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src/usb_init.c
SOURCES += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src/usb_int.c
SOURCES += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src/usb_mem.c
SOURCES += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src/usb_regs.c
SOURCES += $(SRC_ROOT)/platform/stm32l/Libraries/STM32_USB-FS-Device_Driver/src/usb_sil.c
