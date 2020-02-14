# STM32F4xx bsp uart test
REPO_ROOT = ../..

C_SOURCES =  \
main.c \
BlinkerPanel.cpp \
MidiClockDisplay.cpp \
midiTime.cpp \
SegmentVideo.cpp \
sketch.cpp \
StatusPanel.cpp \
os.c

STARTUP_SOURCES = \
$(REPO_ROOT)/bsp/bsp_$(board)/startup_stm32f446xx.s \

SOURCE_DIRS = . \
$(REPO_ROOT)/bsp/bsp_$(board)

# TODO: Not all these should be included maybe
# TODO: FreeRTOSConfig.h location?
C_INCLUDES = \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/bsp/UserProject/Core/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(REPO_ROOT)/bsp/UserProject/AltDrivers \
$(REPO_ROOT)/Arduino/Core \
$(REPO_ROOT)/Arduino/Libs/uCModules/src \
$(REPO_ROOT)/Arduino/Libs/arduino_midi/src \
$(REPO_ROOT)/FreeRTOS \
$(REPO_ROOT)/FreeRTOS/Source/include \
$(REPO_ROOT)/FreeRTOS/Source/CMSIS_RTOS \
$(REPO_ROOT)/FreeRTOS/Source/portable/GCC/ARM_CM4F

STATIC_LIBS = \
$(REPO_ROOT)/Arduino/build/Arduino.a \
$(REPO_ROOT)/bsp/build/bsp.a \
$(REPO_ROOT)/FreeRTOS/build/FreeRTOS.a

#PROJECT_OBJECTS
#OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))
SOURCE_OBJECTS = $(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES)))
STARTUP_OBJECT = $(patsubst %s,%o,$(STARTUP_SOURCES))
OBJECTS = $(SOURCE_OBJECTS) $(STARTUP_OBJECT)
default: all

include $(REPO_ROOT)/tools/build/catalog.mk

include $(REPO_ROOT)/tools/build/common.mk

# linker
$(BUILD_DIR)/output.elf: build_dirs $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS)) $(addprefix $(BUILD_DIR)/obj/, $(STARTUP_OBJECT))
	@echo "#### Linking into elf"
	$(PP) -o $(TARGET_EXEC) $(LDFLAGS) -std=c++11 $(addprefix $(BUILD_DIR)/obj/, $(SOURCE_OBJECTS)) $(STATIC_LIBS) $(addprefix $(BUILD_DIR)/obj/, $(STARTUP_OBJECT))
	$(SZ) $@
	$(OBJD) -S --disassemble $(TARGET_EXEC) > $(TARGET_EXEC).dump
	@echo

$(BUILD_DIR)/output.hex: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to hex"
	$(HEX) $< $@
	@echo

$(BUILD_DIR)/output.bin: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to bin"
	$(BIN) $< $@
	@echo

.PHONY: $(STATIC_LIBS)

all: $(STATIC_LIBS) $(TARGET_EXEC)

clean::
	$(MAKE) -C $(REPO_ROOT)/bsp clean
	$(MAKE) -C $(REPO_ROOT)/Arduino clean
	$(MAKE) -C $(REPO_ROOT)/FreeRTOS clean

openocd:
	cygstart cmd /k $(shell cygpath -w $(PATH_OPENOCD)/launch_openocd.bat)

gdb:
	@echo ""
	@echo "gdb needs special ctrl-c treatment."
	@echo "Launch cmd.exe from start and enter these commands:"
	@echo ""
	@echo "E:"
	@echo "cd $(shell cygpath -w $(REPO_ROOT))modules\midi47fx\test"
	@echo "$(shell cygpath -w $(GDB)) -x gdbinit.txt build/output.elf"

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES) 
