TARGET		:=	cubo_rpg
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	include

ARCH		:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS		:=	-g -Wall -O2 -mword-relocations \
				-fomit-frame-pointer $(ARCH)

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

LDFLAGS		:=	-specs=3dsx.specs $(ARCH)

LIBS		:=	-lcitro3d -lctru -lm

.PHONY: all clean

all: $(TARGET).3dsx

$(TARGET).elf:
	@mkdir -p $(BUILD)
	$(DEVKITARM)/bin/arm-none-eabi-gcc \
		$(CFLAGS) \
		-I$(INCLUDES) \
		-o $(BUILD)/$(TARGET).elf \
		$(SOURCES)/main.c \
		$(LDFLAGS) \
		$(LIBS)

$(TARGET).3dsx: $(TARGET).elf
	3dsxtool $(BUILD)/$(TARGET).elf $(TARGET).3dsx

clean:
	rm -rf $(BUILD) $(TARGET).3dsx
