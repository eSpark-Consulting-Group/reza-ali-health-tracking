
TARGET := nestor_firmware
BOARD := pca10040
SDK_ROOT := ../nRF5_SDK_17.0.2
TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

OUTPUT_DIRECTORY := build

SRC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(patsubst src/%.c,build/%.o,$(SRC_FILES))

INC_FOLDERS := -Iinclude                -Isrc                -I$(SDK_ROOT)/components                -I$(SDK_ROOT)/components/softdevice/s132/headers                -I$(SDK_ROOT)/components/libraries/util                -I$(SDK_ROOT)/components/toolchain/cmsis/include                -I$(SDK_ROOT)/modules/nrfx                -I$(SDK_ROOT)/integration/nrfx/legacy

CFLAGS += -DBOARD_$(BOARD) -DSOFTDEVICE_PRESENT -DNRF52 -mcpu=cortex-m4 -mthumb -mfloat-abi=soft           -Wall -O3 -g3

LDFLAGS += -L$(SDK_ROOT)/components/toolchain/gcc -Tlinker/flash_placement_dfu.ld
LIBS += -lc -lnosys

all: $(OUTPUT_DIRECTORY)/$(TARGET).hex

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_FOLDERS) -c $< -o $@

$(OUTPUT_DIRECTORY)/$(TARGET).elf: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) $(LIBS) -o $@

$(OUTPUT_DIRECTORY)/$(TARGET).hex: $(OUTPUT_DIRECTORY)/$(TARGET).elf
	nrfutil dfu genpkg --application $< --application-version 1 --dev-type 1 --sd-req 0x91 --key-file dfu/private.key $(OUTPUT_DIRECTORY)/$(TARGET).zip

clean:
	rm -rf build/
