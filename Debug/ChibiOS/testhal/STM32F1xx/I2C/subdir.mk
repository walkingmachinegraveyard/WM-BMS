################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/testhal/STM32F1xx/I2C/fake.c \
../ChibiOS/testhal/STM32F1xx/I2C/i2c_pns.c \
../ChibiOS/testhal/STM32F1xx/I2C/lis3.c \
../ChibiOS/testhal/STM32F1xx/I2C/main.c \
../ChibiOS/testhal/STM32F1xx/I2C/tmp75.c 

OBJS += \
./ChibiOS/testhal/STM32F1xx/I2C/fake.o \
./ChibiOS/testhal/STM32F1xx/I2C/i2c_pns.o \
./ChibiOS/testhal/STM32F1xx/I2C/lis3.o \
./ChibiOS/testhal/STM32F1xx/I2C/main.o \
./ChibiOS/testhal/STM32F1xx/I2C/tmp75.o 

C_DEPS += \
./ChibiOS/testhal/STM32F1xx/I2C/fake.d \
./ChibiOS/testhal/STM32F1xx/I2C/i2c_pns.d \
./ChibiOS/testhal/STM32F1xx/I2C/lis3.d \
./ChibiOS/testhal/STM32F1xx/I2C/main.d \
./ChibiOS/testhal/STM32F1xx/I2C/tmp75.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/testhal/STM32F1xx/I2C/%.o: ../ChibiOS/testhal/STM32F1xx/I2C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


