################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/LCD1602.c 

OBJS += \
./Device/LCD1602.o 

C_DEPS += \
./Device/LCD1602.d 


# Each subdirectory must supply rules for building sources it contributes
Device/%.o Device/%.su: ../Device/%.c Device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"C:/Users/ljr05/OneDrive - The University of Nottingham/Desktop/University/Year 3/Y3 Semestar1/PEDSP (EEE2046)/Electronics Project/Codes/LCD1602_Test_Code/Device" -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Device

clean-Device:
	-$(RM) ./Device/LCD1602.d ./Device/LCD1602.o ./Device/LCD1602.su

.PHONY: clean-Device

