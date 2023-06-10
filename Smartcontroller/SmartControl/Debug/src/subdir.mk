################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/config.c \
../src/control.c \
../src/gpio.c \
../src/main.c \
../src/pwm.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/config.o \
./src/control.o \
./src/gpio.o \
./src/main.o \
./src/pwm.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/config.d \
./src/control.d \
./src/gpio.d \
./src/main.d \
./src/pwm.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 '-DHSE_VALUE=8000000' -DSTM32F4 -DSTM32F405VGTx -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/bilal/workspace/SmartControlV1.1sw/SmartControl/StdPeriph_Driver/inc" -I"C:/Users/bilal/workspace/SmartControlV1.1sw/SmartControl/inc" -I"C:/Users/bilal/workspace/SmartControlV1.1sw/SmartControl/CMSIS/device" -I"C:/Users/bilal/workspace/SmartControlV1.1sw/SmartControl/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


