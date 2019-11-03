# STM32F4xx bsp uart test
REPO_ROOT = ../..

C_SOURCES =  \
App.cpp \
BlinkerPanel.cpp \
main.c \
MidiClockDisplay.cpp \
midiTime.cpp \
SegmentVideo.cpp \
sketch.cpp \
StatusPanel.cpp

SOURCE_DIRS = .\

C_INCLUDES = \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/UserProject/Core/Inc \
$(REPO_ROOT)/Arduino/Core \
$(REPO_ROOT)/Arduino/Libs/uCModules/src \
$(REPO_ROOT)/Arduino/Libs/arduino_midi_library/src

STATIC_LIBS = \
$(REPO_ROOT)/bsp/build/bsp.a \
$(REPO_ROOT)/Arduino/build/Arduino.a

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

default: all

include $(REPO_ROOT)/tools/build/catalog.mk

include $(REPO_ROOT)/tools/build/common.mk

.PHONY: $(STATIC_LIBS)

all: $(STATIC_LIBS) $(TARGET_EXEC)

clean::
	$(MAKE) -C $(REPO_ROOT)/bsp clean
	$(MAKE) -C $(REPO_ROOT)/Arduino clean

openocd:
	cygstart cmd /k launch_openocd.bat

deploy:
	cygstart cmd /k launch_gdb.bat
	@echo "For manual use,"
	@echo "cmd $(shell cygpath -w $(CURDIR)/launch_gdb.bat)"
	@echo "Listing batch file contents:"
	@cat launch_gdb.bat

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(TARGET_EXEC) $(STATIC_LIBS)
