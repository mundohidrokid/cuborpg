
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

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard
CFLAGS := -g -Wall -O2 -mword-relocations \
          -ffunction-sections $(ARCH) -D__3DS__

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

LDFLAGS := -specs=3dsx.specs $(ARCH) \
           -Wl,--gc-sections

LIBS := -lcitro3d -lctru -lm

.PHONY: all clean

all: $(TARGET).3dsx

include $(DEVKITARM)/3ds_rules
