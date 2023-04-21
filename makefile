default: all
include makefile_color
include makefile_version

###############################################################################
# Main application file name
###############################################################################
TARGET := $(notdir $(shell pwd))
PHONY :=
MAKEFLAGS += --silent


###############################################################################
# Build path
###############################################################################
ifeq ($(RELEASE), 1)
BUILD_DIR := build/release
else
BUILD_DIR := build/debug
DEBUG := 1
endif

###############################################################################
# Source
###############################################################################
# Defines
C_DEFINES := __EVAL
C_DEFINES += __UVISION_VERSION=530
C_DEFINES += USE_MDR32F9Q1_Rev1
C_DEFINES += _RTE_
C_DEFINES += VERSION_STR=\"$(VERSION_STR)\"
C_DEFINES += VERSION=$(VERSION_NUM)
ifeq ($(RELEASE), 1)
C_DEFINES += PUP_RELEASE
endif

# includes
INCLUDES := $(shell find code -type d)
INCLUDES += MDK-ARM/RTE/Device/MDR1986BE91
INCLUDES += MDK-ARM/RTE/_Target_1

INCLUDES_CC := C:/Users/Artem/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/Core/Include
INCLUDES_CC += C:/Users/Artem/AppData/Local/Arm/Packs/Keil/MDR1986BExx/1.4/Config
INCLUDES_CC += C:/Users/Artem/AppData/Local/Arm/Packs/Keil/MDR1986BExx/1.4/Libraries/CMSIS/CM3/CoreSupport
INCLUDES_CC += C:/Users/Artem/AppData/Local/Arm/Packs/Keil/MDR1986BExx/1.4/Libraries/CMSIS/CM3/DeviceSupport/MDR32F9Qx/inc

# ASM sources
A_SOURCES := $(wildcard $(addsuffix /*.s, $(INCLUDES)))

# C sources
C_SOURCES := $(wildcard $(addsuffix /*.c, $(INCLUDES)))


###############################################################################
# Compilers and Utilities binaries
###############################################################################
PREFIX =
CC_PATH ?= C:/Keil_v5/ARM/ARMCC/Bin

AS 		:= $(CC_PATH)/$(PREFIX)armasm
CC 		:= $(CC_PATH)/$(PREFIX)armcc
LD 		:= $(CC_PATH)/$(PREFIX)armlink
OBJCOPY := $(CC_PATH)/$(PREFIX)fromelf
# OBJDUMP := $(CC_PATH)/$(PREFIX)objdump
# SZ 		:= $(CC_PATH)/$(PREFIX)size
CC_VERSION := $(shell $(CC) --version_number)


###############################################################################
# CFLAGS
###############################################################################
# CPU
CPU := --cpu=Cortex-M3

# FPU
FPU :=

# MCU
MCU := $(CPU) $(FPU)

# Optimization and Debug level
ifeq ($(DEBUG), 1)
C_DEFINES += DEBUG
OPT := -O1 --debug
else
OPT := -O1
endif

# Compile flags
FLAGS := $(MCU)
FLAGS += --apcs=interwork

AFLAGS := $(FLAGS)
AFLAGS += --pd "__EVAL SETA 1"

CFLAGS := $(FLAGS)
CFLAGS += --split_sections
CFLAGS += --gnu
CFLAGS += $(OPT)
CFLAGS += --md

FLAGS_DEF := $(addprefix -D, $(C_DEFINES))
FLAGS_INC := $(addprefix -I, $(INCLUDES)) $(addprefix -I, $(INCLUDES_CC))


###############################################################################
# LFLAGS
###############################################################################
LFLAGS := $(MCU)
LFLAGS += --strict
LFLAGS += --callgraph
LFLAGS += --load_addr_map_info
LFLAGS += --map
LFLAGS += --symbols
LFLAGS += --xref
LFLAGS += --ro_base=0x8000000
LFLAGS += --rw_base=0x20000000
LFLAGS += --first=__Vectors
LFLAGS += --entry=Reset_Handler
LFLAGS += --info=summarysizes,sizes,totals,unused,veneers
LFLAGS += --diag_suppress=9931


###############################################################################
# HFLAGS
###############################################################################
# Options for HEX file generation
HFLAGS := --i32combined


###############################################################################
# build the application
###############################################################################
OBJECTS := $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(A_SOURCES:.s=.o)))
vpath %.c $(sort $(INCLUDES))
vpath %.s $(sort $(INCLUDES))

# Build all

all: build

build: check_cc info link $(BUILD_DIR)/$(TARGET).hex copy_obj #$(BUILD_DIR)/$(TARGET).lst

rebuild: clean build

link: $(OBJECTS) Makefile | $(BUILD_DIR)
	@echo
	$(LD) $(LFLAGS) -o $(BUILD_DIR)/$(TARGET).axf --list=$(BUILD_DIR)/$(TARGET).map $(OBJECTS)

$(BUILD_DIR)/$(TARGET).axf: $(OBJECTS) Makefile
	$(LD) $(LFLAGS) -o $@ --list=$(BUILD_DIR)/$(TARGET).map $(OBJECTS)

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.axf Makefile | $(BUILD_DIR)
	$(OBJCOPY) $(HFLAGS) $< --output $@

$(BUILD_DIR)/%.lst: $(BUILD_DIR)/%.axf Makefile | $(BUILD_DIR)
	$(OBJCOPY) $(CPU) --disassemble $< --output=$@ --interleave=source

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $(CFLAGS) $(FLAGS_INC) $(FLAGS_DEF) $< -o $@ --depend_dir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo $<
	$(AS) $(AFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

copy_obj:
	cp $(BUILD_DIR)/$(TARGET).axf MDK-ARM/Objects/$(TARGET).axf
	cp $(BUILD_DIR)/$(TARGET).hex MDK-ARM/Objects/$(TARGET).hex

clean:
	@echo -rm -fR $(dir $(BUILD_DIR))
	@-rm -fR $(dir $(BUILD_DIR))

utility="C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
flash: $(BUILD_DIR)/$(TARGET).hex
	$(utility) -P $< 0x08000000
	$(utility) -Rst -Run >&-
	@echo $(COLOR_GREEN)"Appication Run"$(COLOR_NC)

erase:
	$(utility) -ME >&-
	@echo $(COLOR_GREEN)"Flash memory erased"$(COLOR_NC)

info: version_info
	$(call echo_green,"Target:") $(TARGET)
	$(call echo_green,"Build to:") $(BUILD_DIR)
	$(call echo_green,Compiler version:) $(CC_VERSION)
	$(call echo_green,Compiler flags:) $(CFLAGS)
	# $(call echo_green,Includes folder:) $(INCLUDES) $(INCLUDES_CC)
	$(call echo_green,Defines:) $(C_DEFINES)
	$(call echo_green,Linking flags:) $(LFLAGS)
	@echo

PHONY += test
test :
	@echo ${BUILD_DIR}
	@echo ${OBJECTS}

PHONY += check_cc
check_cc:
ifeq ($(wildcard $(CC_PATH)),)
	$(error Compiler path: $(CC_PATH) not found)
endif


###############################################################################
# dependencies
###############################################################################
-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: $(PHONY)
