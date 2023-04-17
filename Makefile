CONFIG_PATH=~/.config/vega-tools/settings.mk

include $(CONFIG_PATH)

include $(TAURUS_SDK)/bsp/common/hardware_selector.mk

RISCV_CMODEL=medany
RISCV_LIB_FLAGS= -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)

.PHONY: all clean

all: clean
	autoreconf -f -i bsp
	mkdir build
	cd build && \
	export PATH="$(PATH):$(TAURUS_TOOLCHAIN_PATH)" && \
	../bsp/configure --host=$(TAURUS_COMPILER_PREFIX) \
	--prefix="$(TAURUS_SDK)/build" \
	CFLAGS="$(RISCV_LIB_FLAGS)" && \
	$(MAKE) -C "$(TAURUS_SDK)/build"

clean:	
	rm -rf bin
	rm -rf build

