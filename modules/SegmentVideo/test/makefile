# STM32F4xx bsp uart test
REPO_ROOT = ../../..

ifeq ($(strip $(board)),)
ifneq ('$(MAKECMDGOALS)','clean')
$(info no board specified, setting to clock)
board = clock
endif
endif

#Unused:
#BlinkerPanel.cpp
#MidiClockDisplay.cpp
C_SOURCES =  \
main.cpp \
taskTest.cpp \
taskConsole.cpp \
taskLog.cpp \
os.c

STARTUP_SOURCES =  \
startup_stm32f446xx.s \

SOURCE_DIRS = .\

# TODO: Not all these should be included maybe
# TODO: FreeRTOSConfig.h location?
C_INCLUDES = \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/modules/SegmentVideo \
$(REPO_ROOT)/modules/logging \
$(REPO_ROOT)/FreeRTOS \
$(REPO_ROOT)/FreeRTOS/Source/include \
$(REPO_ROOT)/FreeRTOS/Source/CMSIS_RTOS \
$(REPO_ROOT)/FreeRTOS/Source/portable/GCC/ARM_CM4F

STATIC_LIBS = \
$(REPO_ROOT)/FreeRTOS/build/FreeRTOS.a \
$(REPO_ROOT)/modules/SegmentVideo/build/SegmentVideo.a \
$(REPO_ROOT)/modules/logging/build/logging.a \
$(REPO_ROOT)/bsp/build/bsp_$(board).a

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
	@echo ""
	@echo "Built for $(board) board"
	@echo ""
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

clean:: clean_all

openocd:
	cygstart cmd /k launch_openocd.bat

deploy:
#	cygstart cmd /k launch_gdb.bat
	@echo "For manual use,"
	@echo "cmd $(shell cygpath -w $(CURDIR)/launch_gdb.bat)"
	@echo "Listing batch file contents:"
	@cat launch_gdb.bat

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES) 
