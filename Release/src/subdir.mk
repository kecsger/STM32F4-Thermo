################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Basic.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/Basic.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/Basic.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429VGTx -DSTM32F4 -DSTM32 -DUSE_HAL_DRIVER -DSTM32F429xx -D#GUIDRV_FLEXCOLOR -I"C:/Users/kecsger12/workspace/F429/inc" -I"C:/Users/kecsger12/workspace/F429/CMSIS/core" -I"C:/Users/kecsger12/workspace/F429/CMSIS/device" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc/Legacy" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Config" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/inc" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/kecsger12/workspace/F429/Board/inc" -I"C:/Users/kecsger12/workspace/F429/Fonts/inc" -I"C:/Users/kecsger12/workspace/F429/Application/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


