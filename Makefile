CONFIG_PATH=~/.config/vega-tools/settings.mk

include $(CONFIG_PATH)

include $(TAURUS_SDK)/bsp/common/hardware_selector.mk

RISCV_CMODEL=medany
RISCV_LIB_FLAGS= -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)

.PHONY: all clean

all: clean
	autoreconf -f -i bsp
	cd $(TAURUS_SDK)/bsp && \
	./configure --host=$(TAURUS_COMPILER_PREFIX) CFLAGS="$(RISCV_LIB_FLAGS)"

# FIXME: Build in different directory
	$(MAKE) -C $(TAURUS_SDK)/bsp

clean:	
	rm -rf bin
	rm -f bsp/libtaurus.a

