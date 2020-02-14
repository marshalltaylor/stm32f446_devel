# STM32F4xx bsp uart test
REPO_ROOT = ../..

ifeq ($(strip $(board)),)
ifneq ('$(MAKECMDGOALS)','clean')
$(info no board specified, setting to clock)
board = clock
endif
endif

C_SOURCES =  \
main.c \
App.cpp \
BlinkerPanel.cpp \
midiTime.cpp \
StatusPanel.cpp

STARTUP_SOURCES =  \
startup_stm32f446xx.s \

SOURCE_DIRS = .\

C_INCLUDES = \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/bsp/UserProject/Core/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(REPO_ROOT)/bsp/UserProject/AltDrivers \
$(REPO_ROOT)/modules/logging \
$(REPO_ROOT)/modules/SegmentVideo \
$(REPO_ROOT)/modules/midi47fx \
$(REPO_ROOT)/modules/uCModules


STATIC_LIBS = \
$(REPO_ROOT)/modules/SegmentVideo/build/SegmentVideo.a \
$(REPO_ROOT)/modules/midi47fx/build/midi47fx.a \
$(REPO_ROOT)/modules/uCModules/build/uCModules.a \
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
