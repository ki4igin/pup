###############################################################################
# Build version
###############################################################################
GIT_TAG := $(shell git show-ref --tags --abbrev)
GIT_VER := $(subst v,,$(notdir $(lastword $(GIT_TAG))))
GIT_COMMIT := $(lastword $(filter-out $(lastword $(GIT_TAG)),$(GIT_TAG)))

VERSION := $(GIT_VER)
VERSION_NUM := $(subst ., ,$(VERSION))
ifeq ($(words $(VERSION_NUM)), 3)	
	VERSION_MAJOR := $(word 1, $(VERSION_NUM))
	VERSION_MINOR := $(word 2, $(VERSION_NUM))
	VERSION_REV := $(word 3, $(VERSION_NUM))
	VERSION_NUM := $$((\
		$(VERSION_MAJOR) << 16 + $(VERSION_MINOR) << 8 + $(VERSION_REV)\
	))
else ifeq ($(words $(VERSION_NUM)), 2)	
	VERSION_MAJOR := $(word 1, $(VERSION_NUM))
	VERSION_MINOR := $(word 2, $(VERSION_NUM))
	VERSION_NUM := $$((\
		$(VERSION_MAJOR) * 256 + $(VERSION_MINOR)\
	))
else
	VERSION_NUM := 0
endif

VERSION_COMMIT := $(shell echo $(GIT_COMMIT) | tr a-f A-F)
VERSION_COMMIT := 0x$(VERSION_COMMIT)
VERSION_STR := v$(VERSION) $(VERSION_COMMIT)

all:

test:
	@echo $(PROJECT)$(PROJECT_DIR)$(PROJECT_NAME)

.PHONY: version_info
version_info:
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

PROJECT := $(shell find -name *.uvprojx)
PROJECT_DIR := $(dir $(PROJECT))
PROJECT_NAME := $(patsubst %.uvprojx,%,$(notdir $(PROJECT)))

add_version:	
	@$(call echo_yellow,VERSION_STR:)
	@echo $(VERSION_STR)
	$(shell sed -in "s/VERSION=0x[0-9|A-F]*/VERSION=$(VERSION_COMMIT)/g" $(PROJECT))

remove_version:	
	$(shell sed -in "s/VERSION=0x[0-9|A-F]*/VERSION=0x00000000/g" $(PROJECT))

create_frimware:
	cp $(PROJECT_DIR)Objects/$(PROJECT_NAME).hex ./firmware/$(PROJECT_NAME)_v$(VERSION).hex

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

###############################################################################"