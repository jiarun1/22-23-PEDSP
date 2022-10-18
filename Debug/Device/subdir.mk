################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/Doppler_Radar.c 

OBJS += \
./Device/Doppler_Radar.o 

C_DEPS += \
./Device/Doppler_Radar.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o Device/%.su: ../Device/%.c Device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/CMSIS/DSP/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Device/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Device

clean-Device:
	-$(RM) ./Device/Doppler_Radar.d ./Device/Doppler_Radar.o ./Device/Doppler_Radar.su

.PHONY: clean-Device

