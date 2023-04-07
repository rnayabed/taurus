#-------------------------------------------------------------------- 
#Project Name		: MDP - Microprocessor Development Project
#Project Code		: HD083D
#Created			: 07-Jan-2020
#Filename			: Makefile
#Purpose			: Recursive building of sample programs
#Description		: Recursive building of sample programs
#Author(s)			: Premjith A V
#Email				: premjith@cdac.in
#--------------------------------------------------------------------    
#See LICENSE for license details.

EXAMPLES=./examples
#+++++++++++++++++++++++
# Configurations        
#+++++++++++++++++++++++
# Include the BSP settings

CONFIG_PATH=~/.config/vega-tools/settings.mk
ifeq ("$(wildcard $(CONFIG_PATH))","")
$(error Please install [VEGA SDK]/[VEGA Tools] and setup the environment)
endif

include $(CONFIG_PATH)

ifeq ("$(wildcard $(VEGA_TOOLCHAIN_PATH))","")
$(error Please install [VEGA Tools] and setup the environment)
endif

ifeq ("$(wildcard $(VEGA_SDK))","")
$(error Please install [VEGA SDK] and setup the environment)
endif

.PHONY: default
default: all 

.SILENT: clean all

.PHONY: clean

distclean: clean	
	@rm -f bin/*
	@cd bsp && ./clean.sh
	

clean: $(EXAMPLES)
	for file in $^ ; do \
	echo "--------------------------------------------------------------------------------------------"; \
	$(MAKE) -C $${file} clean ; \
	echo "--------------------------------------------------------------------------------------------"; \
        done
        
.PHONY: all
all: $(EXAMPLES)
	@for file in $^ ; do \
	echo "--------------------------------------------------------------------------------------------"; \
	( $(MAKE)  -C $${file} $@ ) || exit $$?; \
	echo "--------------------------------------------------------------------------------------------"; \
        done
