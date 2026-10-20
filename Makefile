
# CUBO RPG - Build 0.1.0

.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error DEVKITARM is not set)
endif

include $(DEVKITARM)/3ds_rules

TARGET := cubo_rpg
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include
GRAPHICS := gfx
GFXBUILD := $(BUILD)

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS := -g -Wall -O2 -mword-relocations \
          -ffunction-sections $(ARCH) -D__3DS__

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
LDFLAGS := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS := -lcitro3d -lctru -lm
LIBDIRS := $(CTRULIB)

.PHONY: all clean

all:
	$(MAKE) --no-print-directory -f $(DEVKITARM)/3ds_rules

clean:
	rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).elf $(TARGET).smdh
