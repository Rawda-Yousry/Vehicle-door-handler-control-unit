################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GPT/GPT.c 

OBJS += \
./GPT/GPT.o 

C_DEPS += \
./GPT/GPT.d 


# Each subdirectory must supply rules for building sources it contributes
GPT/%.o: ../GPT/%.c GPT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g -DNDEBUG -DSTM32F401xE -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"F:\Study\Embedded\Eclipse\TryProject\Rcc" -I"F:\Study\Embedded\Eclipse\TryProject\Gpio" -I"F:\Study\Embedded\Eclipse\TryProject\GPT" -I"F:\Study\Embedded\Eclipse\TryProject\Lib" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


