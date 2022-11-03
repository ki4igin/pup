###############################################################################
# Main application file name
###############################################################################
TARGET = $(notdir $(shell pwd))

###############################################################################
# Build version
###############################################################################
GIT_TAG = $(shell git show-ref --tags --abbrev)
GIT_VER = $(notdir $(lastword $(GIT_TAG)))
GIT_COMMIT = $(firstword $(shell git show-ref --heads --abbrev))

VERSION = $(GIT_VER)
VERSION_NUM = $(subst ., ,$(VERSION))
ifeq ($(words $(VERSION_NUM)), 3)	
	VERSION_MAJOR = $(word 1, $(VERSION_NUM))
	VERSION_MINOR = $(word 2, $(VERSION_NUM))
	VERSION_REV = $(word 3, $(VERSION_NUM))
	VERSION_NUM = $$((\
		$(VERSION_MAJOR) << 16 + $(VERSION_MINOR) << 8 + $(VERSION_REV)\
	))
else ifeq ($(words $(VERSION_NUM)), 2)	
	VERSION_MAJOR = $(word 1, $(VERSION_NUM))
	VERSION_MINOR = $(word 2, $(VERSION_NUM))
	VERSION_NUM = $$((\
		$(VERSION_MAJOR) * 256 + $(VERSION_MINOR)\
	))
else
	VERSION_NUM = 0
endif

VERSION_COMMIT = 0x$(shell echo $(GIT_COMMIT) | tr a-f A-F)
VERSION_STR = $(VERSION)-$(VERSION_COMMIT)

.PHONY: version_info
version:
	@$(call echo_yellow,GIT_TAG:)
	@echo $(GIT_TAG)
	@$(call echo_yellow,GIT_VER:)
	@echo $(GIT_VER)
	@$(call echo_yellow,GIT_COMMIT:)
	@echo $(GIT_COMMIT)
	@$(call echo_yellow,VERSION:)
	@echo $(VERSION)
	@$(call echo_yellow,VERSION_NUM:)
	@echo $(VERSION_NUM)
	@$(call echo_yellow,VERSION_COMMIT:)
	@echo $(VERSION_COMMIT)
	@$(call echo_yellow,VERSION_STR:)
	@echo $(VERSION_STR)


###############################################################################
# Build path
###############################################################################
ifeq ($(DEBUG), 1)
BUILD_DIR := build/debug
else
BUILD_DIR := build/release
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
# The gcc compiler bin path (default: current folder)
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
CFLAGS += $(addprefix -D, $(C_DEFINES))
CFLAGS += $(addprefix -I, $(INCLUDES)) $(addprefix -I, $(INCLUDES_CC))

###############################################################################
# LFLAGS
###############################################################################
# link script
# LSCRIPT := STM32G431RBTx_FLASH.ld

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
# list of objects
OBJECTS := $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(A_SOURCES:.s=.o)))
vpath %.c $(sort $(INCLUDES))
vpath %.s $(sort $(INCLUDES))

# Build all
all: check_cc build	

build: info link $(BUILD_DIR)/$(TARGET).axf $(BUILD_DIR)/$(TARGET).hex

link: $(OBJECTS)	
	@echo
	$(LD) $(LFLAGS) -o $(BUILD_DIR)/$(TARGET).axf --list=$(BUILD_DIR)/$(TARGET).map $(OBJECTS)
	
$(BUILD_DIR)/$(TARGET).axf: $(OBJECTS)
	@echo	
	$(LD) $(LFLAGS) -o $@ --list=$(BUILD_DIR)/$(TARGET).map $(OBJECTS)

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.axf | $(BUILD_DIR)
	$(OBJCOPY) $(HFLAGS) $< --output $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $(CFLAGS) $< -o $@ --depend_dir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo $<
	$(AS) $(AFLAGS) $< -o $@
	 
$(BUILD_DIR):
	mkdir -p $@

###############################################################################
# clean up
###############################################################################
.PHONY: clean
clean:
	@echo -rm -fR $(dir $(BUILD_DIR))
	-rm -fR $(dir $(BUILD_DIR))

###############################################################################
# Flash
###############################################################################
utility="C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
.PHONY: erase
flash: $(BUILD_DIR)/$(TARGET).hex	
	$(utility) -P $< 0x08000000
	$(utility) -Rst -Run >&-
	@echo $(COLOR_GREEN)"Appication Run"$(COLOR_NC)

erase:
	$(utility) -ME >&-
	@echo $(COLOR_GREEN)"Flash memory erased"$(COLOR_NC)

###############################################################################
# Info
###############################################################################
info:
	$(call echo_yellow,Compile:) \"$(TARGET)\"
	$(call echo_yellow,Version: $(VERSION)) $(VERSION_COMMIT)
	@echo -e $(COLOR_GREEN)"Compiler version:"$(COLOR_NC) $(CC_VERSION)
	@echo -e $(COLOR_GREEN)"Compile flags:"$(COLOR_NC) $(FLAGS)
	@echo -e $(COLOR_GREEN)"Linking flags:"$(COLOR_NC) $(LFLAGS)
	@echo
	# @echo $(OBJECTS)
	# @echo 

.PHONY: test
test : 	
	@echo ${A_SOURCES}

.PHONY: check_cc
check_cc:
ifeq ($(wildcard $(CC_PATH)),)
	$(error Compiler path: $(CC_PATH) not found)
endif	

###############################################################################
# dependencies
###############################################################################
-include $(wildcard $(BUILD_DIR)/*.d)


###############################################################################
# Colors for echo -e
###############################################################################
COLOR_RED		= "\033[0;31m"
COLOR_GREEN		= "\033[0;32m"
COLOR_YELLOW	= "\033[0;33m"
COLOR_NC		= "\033[0m"

###############################################################################
# Colors echo functions
###############################################################################
echo_red = @echo -e $(COLOR_RED)$(1)$(COLOR_NC)
echo_green = @echo -e $(COLOR_GREEN)$(1)$(COLOR_NC)
echo_yellow = @echo -e $(COLOR_YELLOW)$(1)$(COLOR_NC)
