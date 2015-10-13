MCU          = atmega8u2
ARCH         = AVR8
F_CPU        = 8000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = keyboard
VPATH = ../../SmallLibs/blinkmode/
INCLUDE = -I../../SmallLibs/blinkmode/
SRC          = $(TARGET).c Descriptors.c password.c scancode.c random.c eeprom.c blinkmode.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = /home/oscu/lib/lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER $(INCLUDE)
LD_FLAGS     =
AVRDUDE_PROGRAMMER = usbtiny


# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
#include $(LUFA_PATH)/Build/lufa_cppcheck.mk
#include $(LUFA_PATH)/Build/lufa_doxygen.mk
#include $(LUFA_PATH)/Build/lufa_dfu.mk
#include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
#include $(LUFA_PATH)/Build/lufa_atprogram.mk
