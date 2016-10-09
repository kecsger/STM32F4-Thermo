################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/kecsger12/workspace/F429/inc" -I"C:/Users/kecsger12/workspace/F429/CMSIS/core" -I"C:/Users/kecsger12/workspace/F429/CMSIS/device" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc/Legacy" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Config" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/inc" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/kecsger12/workspace/F429/Board/inc" -I"C:/Users/kecsger12/workspace/F429/Fonts/inc" -I"C:/Users/kecsger12/workspace/F429/Application/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


