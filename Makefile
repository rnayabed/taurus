CONFIG_PATH=~/.config/vega-tools/settings.mk

ifeq ("$(wildcard $(CONFIG_PATH))","")
$(error Please install Taurus SDK)
endif

include $(CONFIG_PATH)

ifeq ("$(wildcard $(TAURUS_SDK))","")
$(error Please install [TAURUS_SDK] and setup the environment)
endif

.PHONY: all distclean clean
.SILENT: clean all

all:
	@for file in $^ ; do \
	echo "--------------------------------------------------------------------------------------------"; \
	( $(MAKE)  -C $${file} $@ ) || exit $$?; \
	echo "--------------------------------------------------------------------------------------------"; \
        done

distclean:	
	@rm -f bin/*
	@rm -f bsp/libvega.a
	
clean:
	for file in $^ ; do \
	echo "--------------------------------------------------------------------------------------------"; \
	$(MAKE) -C $${file} clean ; \
	echo "--------------------------------------------------------------------------------------------"; \
        done

