#-------------------------------------------------------------------- 
#Project Name		: MDP - Microprocessor Development Project
#Project Code		: HD083D
#Created		: 07-Jan-2020
#Filename		: Makefile
#Purpose		: Sample hello world program
#Description		: Sample hello program with uart print
#Author(s)		: Premjith A V
#Email			: premjith@cdac.in
#--------------------------------------------------------------------    
#See LICENSE for license details.
 
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
SDK_PATH=${VEGA_SDK}

#+++++++++++++++++++++++
# Executable name
#+++++++++++++++++++++++
EXECUTABLE_NAME=hello


include $(SDK_PATH)/bsp/common/config.mk
	
