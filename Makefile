# you should add a hard link of this file to the directory you want to build
# or find a smarter way to build each directory separately

PROGRAM_NAME := main

SRC_DIR := src
BUILD_DIR := build

USB_PORT := /dev/ttyUSB0

GCC_FLAGS := -mmcu=atmega328p -Wall -Os
OBJCOPY_FLAGS := -R .eeprom -O ihex
AVRDUDE_FLAGS := -c arduino -p m328p -P $(USB_PORT)

OBJECT_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
ELF_FILE := $(BUILD_DIR)/$(PROGRAM_NAME).elf
HEX_FILE := $(BUILD_DIR)/$(PROGRAM_NAME).hex

all: $(HEX_FILE)

# create the build directory
$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

# check if build directory exists
$(OBJECT_FILES): | $(BUILD_DIR)

# compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	avr-gcc -c $(GCC_FLAGS) $< -o $@

# linking
$(ELF_FILE): $(OBJECT_FILES)
	avr-gcc $^ $(GCC_FLAGS) -o $@

# create .hex file from .elf
$(BUILD_DIR)/$(PROGRAM_NAME).hex: $(ELF_FILE)
	avr-objcopy $(OBJCOPY_FLAGS) $^ $@

# flash MCU
flash: $(HEX_FILE)
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$<

# remove build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash clean