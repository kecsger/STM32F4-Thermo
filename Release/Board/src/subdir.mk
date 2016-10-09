################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Board/src/ad7792_temp.c \
../Board/src/ad7843_touch.c \
../Board/src/ili9341.c \
../Board/src/led.c \
../Board/src/pwm.c \
../Board/src/relay.c \
../Board/src/rfm73.c \
../Board/src/spi.c \
../Board/src/spwf01sa_wifi.c \
../Board/src/uart.c 

OBJS += \
./Board/src/ad7792_temp.o \
./Board/src/ad7843_touch.o \
./Board/src/ili9341.o \
./Board/src/led.o \
./Board/src/pwm.o \
./Board/src/relay.o \
./Board/src/rfm73.o \
./Board/src/spi.o \
./Board/src/spwf01sa_wifi.o \
./Board/src/uart.o 

C_DEPS += \
./Board/src/ad7792_temp.d \
./Board/src/ad7843_touch.d \
./Board/src/ili9341.d \
./Board/src/led.d \
./Board/src/pwm.d \
./Board/src/relay.d \
./Board/src/rfm73.d \
./Board/src/spi.d \
./Board/src/spwf01sa_wifi.d \
./Board/src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Board/src/%.o: ../Board/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F429VGTx -DSTM32F4 -DSTM32 -DUSE_HAL_DRIVER -DSTM32F429xx -D#GUIDRV_FLEXCOLOR -I"C:/Users/kecsger12/workspace/F429/inc" -I"C:/Users/kecsger12/workspace/F429/CMSIS/core" -I"C:/Users/kecsger12/workspace/F429/CMSIS/device" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc/Legacy" -I"C:/Users/kecsger12/workspace/F429/HAL_Driver/Inc" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/Config" -I"C:/Users/kecsger12/workspace/F429/Middlewares/ST/STemWin/inc" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/kecsger12/workspace/F429/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/kecsger12/workspace/F429/Board/inc" -I"C:/Users/kecsger12/workspace/F429/Fonts/inc" -I"C:/Users/kecsger12/workspace/F429/Application/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


