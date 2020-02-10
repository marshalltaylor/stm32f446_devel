# STM32F4xx bsp
REPO_ROOT = ..

C_SOURCES =  \
Core/interface.cpp \
Core/abi.cpp \
Core/GPIO.cpp \
Core/ADC.cpp \
Core/HardwareSerial.cpp \
Core/STM_ArduinoPins.cpp \
Core/itoa.cpp \
Core/Print.cpp \
Core/Stream.cpp \
Core/WString.cpp \
Core/STM_ArduinoPins.cpp \
Libs/uCModules/src/flagMessaging.cpp \
Libs/uCModules/src/HardwareInterfaces.cpp \
Libs/uCModules/src/Panel.cpp \
Libs/uCModules/src/PanelComponents.cpp \
Libs/uCModules/src/PanelDataObjects.cpp \
Libs/uCModules/src/timeKeeper.cpp \
Libs/uCModules/src/timeKeeper32.cpp \
Libs/uCModules/src/timerModule.cpp \
Libs/uCModules/src/timerModule32.cpp \
Libs/arduino_midi/src/MIDI.cpp

SOURCE_DIRS = \
Core \
Libs/uCModules/src \
Libs/arduino_midi/src

C_INCLUDES = \
Core \
Libs/uCModules/src \
Libs/arduino_midi/src \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/UserProject/AltDrivers \
$(REPO_ROOT)/bsp/UserProject/Core/Inc

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = Arduino.a

default: info all

include $(REPO_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES)
