# STM32F4xx bsp
REPO_ROOT = ..

C_SOURCES =  \
UserProject/Core/Src/main_bsp.c \
UserProject/Core/Src/gpio.c \
UserProject/Core/Src/adc.c \
UserProject/Core/Src/adc_ext.c \
UserProject/Core/Src/debugPins.c \
UserProject/Core/Src/display_clock.c \
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
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
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
startup_stm32f446xx.s

SOURCE_DIRS = \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src \
UserProject/Core/Src

C_INCLUDES = \
CubeProject/Drivers/CMSIS/Include \
CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
UserProject/Core/Inc

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = bsp.a

default: all

include $(REPO_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo 
