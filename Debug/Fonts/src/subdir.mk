################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fonts/src/font_Arial.c \
../Fonts/src/font_Default.c 

OBJS += \
./Fonts/src/font_Arial.o \
./Fonts/src/font_Default.o 

C_DEPS += \
./Fonts/src/font_Arial.d \
./Fonts/src/font_Default.d 


# Each subdirectory must supply rules for building sources it contributes
Fonts/src/%.o: ../Fonts/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429VGTx -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/kecsger12/workspace/F429/inc" -I"C:/Users/kecsger12/workspace/F429/Board/inc" -I"C:/Users/kecsger12/workspace/F429/CMSIS/core" -I"C:/Users/kecsger12/workspace/F429/CMSIS/device" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc/Legacy" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Config" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Lib" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/inc" -I"C:/Users/kecsger12/workspace/F429/Fonts/inc" -I"C:/Users/kecsger12/workspace/F429/Application/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


