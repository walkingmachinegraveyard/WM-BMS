################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/AVR/hal_lld.c \
../ChibiOS/os/hal/platforms/AVR/i2c_lld.c \
../ChibiOS/os/hal/platforms/AVR/pal_lld.c \
../ChibiOS/os/hal/platforms/AVR/serial_lld.c 

OBJS += \
./ChibiOS/os/hal/platforms/AVR/hal_lld.o \
./ChibiOS/os/hal/platforms/AVR/i2c_lld.o \
./ChibiOS/os/hal/platforms/AVR/pal_lld.o \
./ChibiOS/os/hal/platforms/AVR/serial_lld.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/AVR/hal_lld.d \
./ChibiOS/os/hal/platforms/AVR/i2c_lld.d \
./ChibiOS/os/hal/platforms/AVR/pal_lld.d \
./ChibiOS/os/hal/platforms/AVR/serial_lld.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/AVR/%.o: ../ChibiOS/os/hal/platforms/AVR/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


