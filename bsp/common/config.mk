#+++++++++++++++++++++++
# Toolchain
#+++++++++++++++++++++++
ifndef TAURUS_COMPILER_PREFIX
$(error TAURUS_COMPILER_PREFIX is not set)
endif

RISCV_GCC     := $(TAURUS_COMPILER_PREFIX)-gcc
RISCV_GXX     := $(TAURUS_COMPILER_PREFIX)-g++
RISCV_OBJDUMP := $(TAURUS_COMPILER_PREFIX)-objdump
RISCV_OBJCOPY := $(TAURUS_COMPILER_PREFIX)-objcopy
RISCV_GDB     := $(TAURUS_COMPILER_PREFIX)-gdb
RISCV_AR      := $(TAURUS_COMPILER_PREFIX)-ar
RISCV_SIZE    := $(TAURUS_COMPILER_PREFIX)-size

#+++++++++++++++++++++++
# Flags
#+++++++++++++++++++++++

include $(TAURUS_SDK)/bsp/common/hardware_selector.mk

RISCV_CMODEL=medany
RISCV_LIB_FLAGS= -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)

# Setting up the  arch, ABI, and code model of selected board
RISCV_CCASFLAGS += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CFLAGS    += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CXXFLAGS  += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_ASFLAGS   += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
# Prune unused functions and data
RISCV_CFLAGS   += -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp #-g -Og
RISCV_CXXFLAGS += -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp #-g -Og
# Include baremetal driver headers
RISCV_CCASFLAGS += -I$(TAURUS_SDK)/bsp/include
RISCV_CFLAGS    += -I$(TAURUS_SDK)/bsp/include
RISCV_CXXFLAGS  += -I$(TAURUS_SDK)/bsp/include

RISCV_CCASFLAGS += -I.
RISCV_CFLAGS    += -I.
RISCV_CXXFLAGS  += -I.
# newlib-nano spec
RISCV_CCASFLAGS += --specs=nano.specs -specs=nosys.specs
RISCV_CFLAGS    += --specs=nano.specs -specs=nosys.specs
RISCV_CXXFLAGS  += --specs=nano.specs -specs=nosys.specs

# Linker will remove the unused sections
RISCV_LDFLAGS += -Wl,--gc-sections
# linker map file generation
RISCV_LDFLAGS += -Wl,-Map,$(BIN)/$(EXECUTABLE_NAME).map
# Turn off the C standard library
RISCV_LDFLAGS += -nostartfiles -nostdlib -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
# Find the archive files and linker scripts
RISCV_LDFLAGS += -T $(TAURUS_SDK)/bsp/common/mbl.lds -L$(TAURUS_SDK)/bsp

# Link to the relevant libraries
RISCV_LDLIBS += -Wl,--start-group -Wl,--no-warn-rwx-segments -lvega -lc -lgcc -lm  -Wl,--end-group

# Folders
BIN=build

#+++++++++++++++++++++++
# Rules
#+++++++++++++++++++++++
CC=$(RISCV_GCC)

PROGRAM_ELF ?= $(BIN)/$(EXECUTABLE_NAME).elf
PROGRAM_BIN ?= $(BIN)/$(EXECUTABLE_NAME).bin
PROGRAM_HEX ?= $(BIN)/$(EXECUTABLE_NAME).hex
PROGRAM_LST ?= $(BIN)/$(EXECUTABLE_NAME).lst
PROGRAM_DUMP ?= $(BIN)/$(EXECUTABLE_NAME).dump


sources_shell_c :=  $(shell find . -name "*.c")
$(info C Files : [${sources_shell_c}])
OBJECT_FILES_C := $(patsubst %.c,$(BIN)/%.o,$(sources_shell_c))

sources_shell_S :=  $(shell find . -name "*.S")
$(info Assembly Files : [${sources_shell_S}])
OBJECT_FILES_S := $(patsubst %.S,$(BIN)/%.o,$(sources_shell_S))


#OBJECT_FILES_C   = $(patsubst %.c, $(BIN)/%.o,  $(wildcard *.c */*.c))
#OBJECT_FILES_S   = $(patsubst %.S, $(BIN)/%.o,  $(wildcard *.S */*.S))

 
.PHONY: default selected_hardware clean distclean mrproper
 
#upload: all
#ifeq ($(MACHINE),THEJAS32)
#	@$(UTIL_PATH)/xmodem /dev/ttyUSB0 $(PWD)/$(BIN)/$(EXECUTABLE_NAME).bin
#	#@echo "Please connect the aries board to PC and enter your password to open minicom";	
#	sudo minicom aries
#else
#	@$(UTIL_PATH)/eth_transfer/send.sh $(PWD)/$(BIN)/$(EXECUTABLE_NAME).bin
#endif	

default: selected_hardware $(PROGRAM_ELF) 

selected_hardware:
	$(info Current Hardware Selected: $(VEGA_MACHINE))

clean:
	rm -rf $(BIN)
	rm -rf $(TAURUS_SDK)/bin/$(EXECUTABLE_NAME).bin

.PHONY: build clean

$(PROGRAM_ELF): $(OBJECT_FILES_C) $(OBJECT_FILES_S) 
	$(info Linking $^)
	$(CC) $(RISCV_LDFLAGS) $^ $(RISCV_LDLIBS) -o $@  	

	$(info =======================================)
	$(info Build successful!)
	@$(RISCV_OBJDUMP) -d $@ > $(PROGRAM_DUMP)
	@$(RISCV_OBJCOPY) -I elf$(XLEN)-littleriscv -O binary  $@ $(PROGRAM_BIN)
#	@$(UTIL_PATH)/bin2hex --bit-width 128 $(PROGRAM_BIN) $(PROGRAM_HEX)
	@$(RISCV_OBJDUMP) --source --all-headers --demangle --line-numbers --wide $@ > $(PROGRAM_LST)
	$(info ELF: $(EXECUTABLE_NAME).elf)
	$(info Binary: $(EXECUTABLE_NAME).bin)
# FIXME: Work needed	
#	$(info Hex: $(EXECUTABLE_NAME).hex)
	$(info Dump: $(EXECUTABLE_NAME).dump)
	$(info Files are generated in $(BIN) folder.)
	$(info Size information)
	@$(RISCV_SIZE) $@

	@mkdir -p $(TAURUS_SDK)/bin
	@cp $(PROGRAM_BIN) $(TAURUS_SDK)/bin/
	
$(OBJECT_FILES_C): $(BIN)/%.o: %.c
	@echo Compiling $<
	@mkdir -p $(@D)	
	$(CC) $(RISCV_CFLAGS) -c $^ -o $@

$(OBJECT_FILES_S): $(BIN)/%.o: %.S
	@echo Compiling $<	
	@mkdir -p $(@D)	
	$(CC) $(RISCV_CCASFLAGS) -o $@ $<	
