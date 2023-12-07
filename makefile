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


LIB_DIR := drivers

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
INCLUDES := $(shell find ./code -type d)
INCLUDES_LIB := $(shell find -name $(LIB_DIR))
INCLUDES_LIB += $(shell find -name MDR1986BE91)
INCLUDES_LIB += $(shell find -name paralax)
INCLUDES_SRC := $(filter-out $(INCLUDES_LIB),$(INCLUDES))
INCLUDES := $(INCLUDES_SRC) $(INCLUDES_LIB)

INCLUDES_CC := C:/Users/Artem/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/Core/Include
INCLUDES_CC += C:/Users/Artem/AppData/Local/Arm/Packs/Keil/MDR1986BExx/1.4/Config
INCLUDES_CC += C:/Users/Artem/AppData/Local/Arm/Packs/Keil/MDR1986BExx/1.4/Libraries/CMSIS/CM3/CoreSupport
INCLUDES_CC += C:/Users/Artem/AppData/Local/Arm/Packs/Keil/MDR1986BExx/1.4/Libraries/CMSIS/CM3/DeviceSupport/MDR32F9Qx/inc

# ASM sources
SOURCES_A := $(wildcard $(addsuffix /*.s, $(INCLUDES)))

# C sources
SOURCES_C := $(wildcard $(addsuffix /*.c, $(INCLUDES_SRC)))

# C lib sources
SOURCES_C_LIB := $(wildcard $(addsuffix /*.c, $(INCLUDES_LIB)))


###############################################################################
# Compilers and Utilities binaries
###############################################################################
PREFIX =
CC_PATH ?= C:/Keil_v5/ARM/ARMCLANG/Bin

AS 		:= $(CC_PATH)/$(PREFIX)armasm
CC 		:= $(CC_PATH)/$(PREFIX)armclang
LD 		:= $(CC_PATH)/$(PREFIX)armlink
OBJCOPY := $(CC_PATH)/$(PREFIX)fromelf
# OBJDUMP := $(CC_PATH)/$(PREFIX)objdump
# SZ 		:= $(CC_PATH)/$(PREFIX)size
CC_VERSION := $(shell $(CC) --version_number)


###############################################################################
# CFLAGS
###############################################################################
# CPU
CPU := -mcpu=cortex-m3 

# FPU
FPU :=

# MCU
MCU := $(CPU) $(FPU)

# Optimization and Debug level
ifeq ($(DEBUG), 1)
DEFINES_C += DEBUG
OPT := -O1 --debug
else
OPT := -O1
endif

# Compile flags
FLAGS := $(MCU)
FLAGS += --target=arm-arm-none-eabi 

FLAGS_A := --cpu Cortex-M3
FLAGS_A += --pd "__EVAL SETA 1"

FLAGS_C := $(FLAGS)
FLAGS_C += -xc
FLAGS_C += -std=gnu11
FLAGS_C += $(OPT)
FLAGS_C += -fno-rtti 
FLAGS_C += -funsigned-char 
FLAGS_C += -fshort-enums 
FLAGS_C += -fshort-wchar
FLAGS_C += -fno-ldm-stm 
FLAGS_C += -ffunction-sections 
FLAGS_C += -MD

FLAGS_C_W := -Weverything
FLAGS_C_W += -Wno-reserved-id-macro
FLAGS_C_W += -Wno-unused-macros
FLAGS_C_W += -Wno-missing-noreturn
FLAGS_C_W += -Wno-documentation
FLAGS_C_W += -Wno-sign-conversion
# FLAGS_C_W += -Wno-implicit-int-conversion
# FLAGS_C_W += -Wno-double-promotion
FLAGS_C_W += -Wno-cast-align
FLAGS_C_W += -Wno-padded

FLAGS_DEF := $(addprefix -D, $(DEFINES_C))
FLAGS_INC := $(addprefix -I, $(INCLUDES)) $(addprefix -I, $(INCLUDES_CC))


###############################################################################
# FLAGS_L
###############################################################################
FLAGS_L := --cpu Cortex-M3
FLAGS_L += --ro-base 0x08000000
FLAGS_L += --entry 0x08000000
FLAGS_L += --rw-base 0x20000000
FLAGS_L += --entry Reset_Handler
FLAGS_L += --first __Vectors
FLAGS_L += --strict 
FLAGS_L += --summary_stderr 
FLAGS_L += --info summarysizes



###############################################################################
# HFLAGS
###############################################################################
# Options for HEX file generation
HFLAGS := --i32combined


###############################################################################
# build the application
###############################################################################
OBJECTS := $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_C:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_C_LIB:.c=.lo)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_A:.s=.o)))
vpath %.c $(sort $(INCLUDES))
vpath %.s $(sort $(INCLUDES))

# Build all

all: build

build: check_cc info link $(BUILD_DIR)/$(TARGET).hex copy_obj

rebuild: clean build

link: $(OBJECTS) Makefile | $(BUILD_DIR)
	@echo
	$(LD) $(FLAGS_L) -o $(BUILD_DIR)/$(TARGET).axf --list=$(BUILD_DIR)/$(TARGET).map $(OBJECTS)

$(BUILD_DIR)/$(TARGET).axf: $(OBJECTS) Makefile
	$(LD) $(FLAGS_L) -o $@ --list=$(BUILD_DIR)/$(TARGET).map $(OBJECTS)

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.axf Makefile | $(BUILD_DIR)
	$(OBJCOPY) $(HFLAGS) $< --output $@

$(BUILD_DIR)/%.lo: %.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $(FLAGS_C) $(FLAGS_INC) $(FLAGS_DEF) $< -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $(FLAGS_C) $(FLAGS_C_W) $(FLAGS_INC) $(FLAGS_DEF) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo $<
	$(AS) $(FLAGS_A) $< -o $@

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
	$(call echo_green,Compiler flags:) $(FLAGS_C)
	$(call echo_green,Includes folder:) $(INCLUDES) $(INCLUDES_CC)
	$(call echo_green,Defines:) $(DEFINES_C)
	$(call echo_green,Linking flags:) $(FLAGS_L)
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
