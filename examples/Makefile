#-------------------------------------------------------------------- 
#Project Name		: MDP - Microprocessor Development Project
#Project Code		: HD083D
#Created		: 07-Jan-2020
#Filename		: Makefile
#Purpose		: Recursive building of sample programs
#Description		: Recursive building of sample programs
#Author(s)		: Premjith A V
#Email			: premjith@cdac.in
#--------------------------------------------------------------------    
#See LICENSE for license details.

EXAMPLES=$(shell find . -mindepth 2 -name 'Makefile'|xargs dirname)

.PHONY: default
default: all 

.SILENT: clean all

.PHONY: clean
clean: $(EXAMPLES)
	for file in $^ ; do \
	echo "--------------------------------------------------------------------------------------------"; \
	$(MAKE) -C $${file} clean ; \
	echo "--------------------------------------------------------------------------------------------"; \
        done
        
.PHONY: all
all: $(EXAMPLES)
	for file in $^ ; do \
	echo "--------------------------------------------------------------------------------------------"; \
	$(MAKE) -C $${file} ; \
	echo "--------------------------------------------------------------------------------------------"; \
        done
