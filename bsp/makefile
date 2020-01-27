# STM32F4xx bsp
REPO_ROOT = ..

# select board
$(info board='$(board)')

ifeq ('$(board)','clock')
C_SOURCES_BOARD := \
bsp_clock/bspSys.c \
bsp_clock/bspIO.c \
bsp_clock/bspSerial.c \
bsp_clock/bspSPI.c \
bsp_clock/bspTime.c \
bsp_clock/bspADC.c

else ifeq ('$(board)','dot2')
C_SOURCES_BOARD := \
bsp_dot2/bspSys.c \
bsp_dot2/bspIO.c \
bsp_dot2/bspSerial.c \
bsp_dot2/bspSPI.c \
bsp_dot2/bspTime.c \
bsp_dot2/bspADC.c \
bsp_dot2/bspPins.c

# allow no board for clean target
else ifeq ('$(MAKECMDGOALS)','clean')
C_SOURCES_BOARD :=

else
# Stop
$(error board variable not defined!  ex 'make board=dot2')
endif

C_SOURCES = $(C_SOURCES_BOARD) \
UserProject/Core/Src/main_cubemx.c \
UserProject/Core/Src/gpio.c \
UserProject/Core/Src/adc.c \
UserProject/Core/Src/debugUtilities.c \
UserProject/Core/Src/dma.c \
UserProject/Core/Src/spi.c \
UserProject/Core/Src/tim.c \
UserProject/Core/Src/usart.c \
UserProject/Core/Src/stm32f4xx_it.c \
UserProject/Core/Src/stm32f4xx_hal_msp.c \
UserProject/Core/Src/unused_int.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
UserProject/AltDrivers/custom_stm32f4xx_hal_uart.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
UserProject/Core/Src/system_stm32f4xx.c \
UserProject/Core/Src/stm32f4xx_hal_timebase_TIM.c

# List of directories which contain C_SOURCES.
# This list is created within the BUILD_DIR and used for artifacts
SOURCE_DIRS = .\
bsp_$(board) \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src \
UserProject/AltDrivers \
UserProject/Core/Src

C_INCLUDES = .\
bsp_$(board) \
UserProject/AltDrivers \
UserProject/Core/Inc \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
CubeProject/Drivers/CMSIS/Include \
CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = bsp_$(board).a

default: all

include $(REPO_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
#	@echo $(BUILD_PATHS)
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
#	@echo $(addprefix -I,$(C_INCLUDES))
