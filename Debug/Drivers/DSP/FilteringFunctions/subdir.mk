################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.c \
../Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.c 

C_DEPS += \
./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.d \
./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.d 

OBJS += \
./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.o \
./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DSP/FilteringFunctions/%.o Drivers/DSP/FilteringFunctions/%.su Drivers/DSP/FilteringFunctions/%.cyclo: ../Drivers/DSP/FilteringFunctions/%.c Drivers/DSP/FilteringFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L452xx -c -I../Core/Inc -I../Drivers/AI -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DSP-2f-FilteringFunctions

clean-Drivers-2f-DSP-2f-FilteringFunctions:
	-$(RM) ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.cyclo ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.d ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.o ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_f32.su ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.cyclo ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.d ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.o ./Drivers/DSP/FilteringFunctions/arm_biquad_cascade_df1_init_f32.su

.PHONY: clean-Drivers-2f-DSP-2f-FilteringFunctions

