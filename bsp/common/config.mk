MACHINE=${VEGA_MACHINE}
UTIL_PATH=${VEGA_TOOLS}/utils


ifeq ($(MACHINE),THEJAS32)
$(info TARGET:  THEJAS32 Hardware)
RISCV_ARCH=rv32im
RISCV_ABI=ilp32
XLEN=32
RISCV_CMODEL=medany
TOOLCHAIN_PATH=${VEGA_TOOLCHAIN_PATH}
RISCV_LIB_FLAGS= -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
endif
ifeq ($(MACHINE),THEJAS64)
$(info TARGET:  THEJAS64 Hardware)
RISCV_ARCH=rv64ima
RISCV_ABI=lp64
XLEN=64
RISCV_CMODEL=medany
TOOLCHAIN_PATH=${VEGA_TOOLCHAIN_PATH}
RISCV_LIB_FLAGS= -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
endif
ifeq ($(MACHINE),CDAC)
$(info TARGET: CDAC FPGA Board)
RISCV_ARCH=rv64imafd
RISCV_ABI=lp64d
XLEN=64
RISCV_CMODEL=medany
TOOLCHAIN_PATH=${VEGA_TOOLCHAIN_PATH}
RISCV_LIB_FLAGS= -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
endif

#+++++++++++++++++++++++
# Toolchain
#+++++++++++++++++++++++

# Please add your own compiler prefix here
CROSS_COMPILE ?= riscv64-vega-elf

# Add the custom toolchain path in VEGA_RISCV_PATH
ifeq ($(VEGA_RISCV_PATH),)
RISCV_GCC     := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-gcc
RISCV_GXX     := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-g++
RISCV_OBJDUMP := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-objdump
RISCV_OBJCOPY := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-objcopy
RISCV_GDB     := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-gdb
RISCV_AR      := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-ar
RISCV_SIZE    := $(TOOLCHAIN_PATH)/$(CROSS_COMPILE)-size
PATH          := $(TOOLCHAIN_PATH)):$(PATH)
else
RISCV_GCC     := $(CROSS_COMPILE)-gcc
RISCV_GXX     := $(CROSS_COMPILE)-g++
RISCV_OBJDUMP := $(CROSS_COMPILE)-objdump
RISCV_OBJCOPY := $(CROSS_COMPILE)-objcopy
RISCV_GDB     := $(CROSS_COMPILE)-gdb
RISCV_AR      := $(CROSS_COMPILE)-ar
RISCV_SIZE    := $(CROSS_COMPILE)-size
endif
#+++++++++++++++++++++++
# Flags
#+++++++++++++++++++++++

# Setting up the  arch, ABI, and code model of selected board
RISCV_CCASFLAGS += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CFLAGS    += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CXXFLAGS  += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_ASFLAGS   += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
# Prune unused functions and data
RISCV_CFLAGS   += -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp #-g -Og
RISCV_CXXFLAGS += -fno-builtin-printf -fno-builtin-puts -fdata-sections -ffunction-sections -fno-builtin-memcmp #-g -Og
# Include baremetal driver headers
RISCV_CCASFLAGS += -I$(SDK_PATH)/bsp/include
RISCV_CFLAGS    += -I$(SDK_PATH)/bsp/include
RISCV_CXXFLAGS  += -I$(SDK_PATH)/bsp/include

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
RISCV_LDFLAGS += -T $(SDK_PATH)/bsp/common/mbl.lds -L$(SDK_PATH)/bsp

# Link to the relevant libraries
RISCV_LDLIBS += -Wl,--start-group -lvega -lc -lgcc -lm  -Wl,--end-group

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


.PHONY: all

default: all 


upload: all
ifeq ($(MACHINE),THEJAS32)
	@$(UTIL_PATH)/xmodem /dev/ttyUSB0 $(PWD)/$(BIN)/$(EXECUTABLE_NAME).bin
	#@echo "Please connect the aries board to PC and enter your password to open minicom";	
	sudo minicom aries
else
	@$(UTIL_PATH)/eth_transfer/send.sh $(PWD)/$(BIN)/$(EXECUTABLE_NAME).bin
endif	


all:   build_vega_lib $(PROGRAM_ELF) 

clean:
	rm -rf $(BIN)/*
	@rm -rf $(SDK_PATH)/bin/$(EXECUTABLE_NAME).bin

distclean:
	rm -rf -f $(BIN)	
	@rm -rf $(SDK_PATH)/bin/$(EXECUTABLE_NAME).bin
	@cd $(SDK_PATH)/bsp/ && ./clean.sh

mrproper:
	rm -r -f $(BIN)	
	@rm -f $(SDK_PATH)/bin/$(EXECUTABLE_NAME).bin
	@cd $(SDK_PATH)/bsp/ && ./clean.sh

.PHONY: build clean

$(PROGRAM_ELF): $(OBJECT_FILES_C) $(OBJECT_FILES_S) 
	@echo Linking $^
	$(CC) $(RISCV_LDFLAGS)  $^ $(RISCV_LDLIBS) -o $@  	
	@echo "Build successful!"
	@$(RISCV_OBJDUMP) -d $@ > $(PROGRAM_DUMP)
	@$(RISCV_OBJCOPY) -I elf$(XLEN)-littleriscv -O binary  $@ $(PROGRAM_BIN)
	@$(UTIL_PATH)/bin2hex --bit-width 128 $(PROGRAM_BIN) $(PROGRAM_HEX)
	@$(RISCV_OBJDUMP) --source --all-headers --demangle --line-numbers --wide $@ > $(PROGRAM_LST)
	@echo -n "ELF\t: $(EXECUTABLE_NAME)\nBinary\t: $(EXECUTABLE_NAME).bin\n"
	@echo -n "Hex\t: $(EXECUTABLE_NAME).hex\nDump\t: $(EXECUTABLE_NAME).dump\nFiles are generated in $(BIN) folder.\n"
	@echo -n "Size information\n"
	@$(RISCV_SIZE) $@
	@mkdir -p $(SDK_PATH)/bin
	@cp $(PROGRAM_BIN) $(SDK_PATH)/bin/
	
$(OBJECT_FILES_C): $(BIN)/%.o: %.c
	@echo Compiling $<
	@mkdir -p $(@D)	
	$(CC) $(RISCV_CFLAGS) -c $^ -o $@

$(OBJECT_FILES_S): $(BIN)/%.o: %.S
	@echo Compiling $<	
	@mkdir -p $(@D)	
	$(CC) $(RISCV_CCASFLAGS) -o $@ $<	
	
build_vega_lib: 
	@cd $(SDK_PATH)/bsp/ &&  ./setup.sh "$(TOOLCHAIN_PATH)" "$(RISCV_LIB_FLAGS)"
	
