# STM32F4xx bsp
REPO_ROOT = ../..

C_SOURCES = \
src/SegmentVideo.cpp \
src/MidiClockDisplay.cpp

SOURCE_DIRS = src

C_INCLUDES = . \
src \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/UserProject/AltDrivers \
$(REPO_ROOT)/bsp/UserProject/Core/Inc \
$(REPO_ROOT)/Arduino/Core \


#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = SegmentVideo.a

default: info all

include $(REPO_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES)
	@echo $(OBJECTS)
