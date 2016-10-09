################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STemWin/Config/GUIConf.c \
../Middlewares/ST/STemWin/Config/GUIDRV_Template.c \
../Middlewares/ST/STemWin/Config/LCDConf.c 

OBJS += \
./Middlewares/ST/STemWin/Config/GUIConf.o \
./Middlewares/ST/STemWin/Config/GUIDRV_Template.o \
./Middlewares/ST/STemWin/Config/LCDConf.o 

C_DEPS += \
./Middlewares/ST/STemWin/Config/GUIConf.d \
./Middlewares/ST/STemWin/Config/GUIDRV_Template.d \
./Middlewares/ST/STemWin/Config/LCDConf.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STemWin/Config/%.o: ../Middlewares/ST/STemWin/Config/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429VGTx -DUSE_HAL_DRIVER -DSTM32F429xx -I"C:/Users/kecsger12/workspace/F429/inc" -I"C:/Users/kecsger12/workspace/F429/Board/inc" -I"C:/Users/kecsger12/workspace/F429/CMSIS/core" -I"C:/Users/kecsger12/workspace/F429/CMSIS/device" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc/Legacy" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Config" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Lib" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/inc" -I"C:/Users/kecsger12/workspace/F429/Fonts/inc" -I"C:/Users/kecsger12/workspace/F429/Application/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


