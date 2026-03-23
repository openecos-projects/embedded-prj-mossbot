# ==============================================================================
# Configuration Variables
# These can be overridden from the command line (e.g., make APP_TYPE=sdram)
# ==============================================================================

# Application Name
APP_NAME ?= v1
# Build Type: flash, mem, or sdram
APP_TYPE ?= flash
# Architecture definition
APP_ARCH ?= riscv32e-mycpu

# Environment Variable checks
ifndef AM_HOME
$(error AM_HOME is not set. Please set the AM_HOME environment variable)
endif

# Derived File Names
APP_ORG_BIN := $(APP_NAME)-$(APP_ARCH).bin
APP_ORG_ELF := $(APP_NAME)-$(APP_ARCH).elf
APP_LOD_BIN := loader-$(APP_ARCH).bin
APP_LOD_ELF := loader-$(APP_ARCH).elf
APP_STD_BIN := $(APP_NAME)-$(APP_TYPE).bin
APP_STD_ELF := $(APP_NAME)-$(APP_TYPE).elf
APP_STD_SIM := $(APP_NAME).$(APP_TYPE)
APP_STD_TXT := $(APP_NAME)-$(APP_TYPE).txt

# Directories
HOME_DIR := $(shell pwd)
BIN_OUT_DIR := $(HOME_DIR)/../bin/$(APP_TYPE)

# ==============================================================================
# Helper Macros (mimicking Python functions)
# ==============================================================================

# Function to change linker script type in AM_HOME scripts
# Usage: $(call chg_ld_script, <type>)
define chg_ld_script
	@echo "Configuring LD script for $(1)..."
	sed -i 's/core_[a-z]\+/core_$(1)/' $(AM_HOME)/scripts/$(APP_ARCH).mk
endef

# Function to change memory address in AM_HOME scripts
# Usage: $(call chg_ld_addr, <address>)
define chg_ld_addr
	@echo "Configuring Linker Address to $(1)..."
	sed -i 's/\(pmem_start=\)0x[0-9A-Z]\+/\1$(1)/' $(AM_HOME)/scripts/$(APP_ARCH).mk
endef

# ==============================================================================
# Main Targets
# ==============================================================================

.PHONY: all flash mem sdram clean help

# Default target: routes based on APP_TYPE variable
all: $(APP_TYPE)

# ------------------------------------------------------------------------------
# FLASH Build Target
# ------------------------------------------------------------------------------
flash:
	@echo ">>> Starting FLASH build for $(APP_NAME)..."
	$(call chg_ld_script,flash)
	$(call chg_ld_addr,0x30000000)

	@# Check if APP_NAME is rtthread to decide build method
ifneq ($(APP_NAME),rtthread)
	$(MAKE) -C $(APP_NAME) ARCH=$(APP_ARCH)
else
	@# rt-thread specific build steps
	@echo "Building rtthread..."
	cp $(APP_NAME)/*.c $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/applications/
	cp $(APP_NAME)/*.cc $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/applications/
	cp $(APP_NAME)/*.h $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/applications/

	cd $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/ && \
	sed -i 's/^FLASH = 0/FLASH = 1/' rtconfig.py && \
	scons && \
	cp rtthread.bin $(HOME_DIR)/$(APP_NAME)/build/$(APP_ORG_BIN) && \
	cp rtthread.elf $(HOME_DIR)/$(APP_NAME)/build/$(APP_ORG_ELF)
endif

	@# Post-processing (rename and move)
	mv $(APP_NAME)/build/$(APP_ORG_BIN) $(APP_NAME)/build/$(APP_STD_BIN)
	mv $(APP_NAME)/build/$(APP_ORG_ELF) $(APP_NAME)/build/$(APP_STD_ELF)
	mkdir -p $(BIN_OUT_DIR)
	cp $(APP_NAME)/build/$(APP_STD_BIN) $(BIN_OUT_DIR)/
	cp $(APP_NAME)/build/$(APP_STD_ELF) $(BIN_OUT_DIR)/

	@# Generate Verilog simulation file and disassembly
	cd $(BIN_OUT_DIR) && \
	riscv64-unknown-elf-objcopy -I binary -O verilog $(APP_STD_BIN) $(APP_STD_SIM) && \
	riscv64-unknown-elf-objdump -d $(APP_STD_ELF) > $(APP_STD_TXT)
	@echo ">>> FLASH build complete."

# ------------------------------------------------------------------------------
# MEM / SDRAM Build Logic (Shared Logic)
# ------------------------------------------------------------------------------
# Define addresses based on target
ifeq ($(APP_TYPE),mem)
    TARGET_ADDR := 0x0F000000
else ifeq ($(APP_TYPE),sdram)
    TARGET_ADDR := 0xA0000000
endif

# Logic for both mem and sdram
mem sdram:
	@echo ">>> Starting $(APP_TYPE) build for $(APP_NAME)..."
	$(call chg_ld_script,$(APP_TYPE))
	$(call chg_ld_addr,$(TARGET_ADDR))

	@# 1. Build the Application
ifneq ($(APP_NAME),rtthread)
	$(MAKE) -C $(APP_NAME) ARCH=$(APP_ARCH) LOAD_TYPE=-DMEM_LOAD
	cp $(APP_NAME)/build/$(APP_ORG_BIN) $(HOME_DIR)/loader/
else
	@echo "Building rtthread for $(APP_TYPE)..."
	cp $(APP_NAME)/*.c $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/applications/
	cp $(APP_NAME)/*.cc $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/applications/
	cp $(APP_NAME)/*.h $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/applications/

	cd $(APP_NAME)/rt-thread/bsp/qemu-riscv-virt64/ && \
	sed -i 's/^FLASH = 1/FLASH = 0/' rtconfig.py && \
	scons && \
	cp rtthread.bin $(HOME_DIR)/loader/$(APP_ORG_BIN)
endif

	@# 2. Build the Loader (which includes the App binary)
	@echo "Building Loader..."
	$(call chg_ld_script,flash)
	$(call chg_ld_addr,0x30000000)

	cd loader && \
	sed -i 's/^\(BIN_PATH\s\+=\s\+\)\(.\+\)/\1$(APP_NAME)-$(APP_ARCH)\.bin/' Makefile && \
	$(MAKE) ARCH=$(APP_ARCH)

	@# 3. Post-processing (Loader becomes the standard bin)
	mv loader/build/$(APP_LOD_BIN) loader/build/$(APP_STD_BIN)
	mv loader/build/$(APP_LOD_ELF) loader/build/$(APP_STD_ELF)

	mkdir -p $(BIN_OUT_DIR)
	cp loader/build/$(APP_STD_BIN) $(BIN_OUT_DIR)/
	cp loader/build/$(APP_STD_ELF) $(BIN_OUT_DIR)/

	@# Generate Verilog simulation file and disassembly
	cd $(BIN_OUT_DIR) && \
	riscv64-unknown-elf-objcopy -I binary -O verilog $(APP_STD_BIN) $(APP_STD_SIM) && \
	riscv64-unknown-elf-objdump -d $(APP_STD_ELF) > $(APP_STD_TXT)
	@echo ">>> $(APP_TYPE) build complete."

# ------------------------------------------------------------------------------
# Cleaning
# ------------------------------------------------------------------------------
clean:
	@echo "Cleaning up..."
	-$(MAKE) -C $(APP_NAME) clean
	@if [ -d loader ]; then $(MAKE) -C loader clean; fi
	rm -rf $(BIN_OUT_DIR)

# ------------------------------------------------------------------------------
# Help Target
# ------------------------------------------------------------------------------
help:
	@echo "RISC-V Application Build System"
	@echo "================================"
	@echo "Available targets:"
	@echo "  all       - Build for current APP_TYPE (default: $(APP_TYPE))"
	@echo "  flash     - Build for flash memory"
	@echo "  mem       - Build for memory"
	@echo "  sdram     - Build for SDRAM"
	@echo "  clean     - Remove all build artifacts"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Configuration variables:"
	@echo "  APP_NAME  - Application name (default: $(APP_NAME))"
	@echo "  APP_TYPE  - Memory type: flash, mem, sdram (default: $(APP_TYPE))"
	@echo "  APP_ARCH  - Architecture (default: $(APP_ARCH))"
	@echo "  AM_HOME   - Project root directory (must be set)"
	@echo ""
	@echo "Usage examples:"
	@echo "  make APP_TYPE=flash        # Build for flash memory"
	@echo "  make APP_TYPE=mem          # Build for memory"
	@echo "  make APP_NAME=myapp        # Build a different application"
	@echo "  make clean                 # Clean all build artifacts"
