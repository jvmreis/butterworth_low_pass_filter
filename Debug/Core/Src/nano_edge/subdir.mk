################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/nano_edge/NanoEdgeAI.c 

C_DEPS += \
./Core/Src/nano_edge/NanoEdgeAI.d 

OBJS += \
./Core/Src/nano_edge/NanoEdgeAI.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/nano_edge/%.o Core/Src/nano_edge/%.su Core/Src/nano_edge/%.cyclo: ../Core/Src/nano_edge/%.c Core/Src/nano_edge/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Core/Inc -I../Drivers/AI -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-nano_edge

clean-Core-2f-Src-2f-nano_edge:
	-$(RM) ./Core/Src/nano_edge/NanoEdgeAI.cyclo ./Core/Src/nano_edge/NanoEdgeAI.d ./Core/Src/nano_edge/NanoEdgeAI.o ./Core/Src/nano_edge/NanoEdgeAI.su

.PHONY: clean-Core-2f-Src-2f-nano_edge

