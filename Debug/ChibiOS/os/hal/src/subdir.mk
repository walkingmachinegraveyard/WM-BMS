################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/src/adc.c \
../ChibiOS/os/hal/src/can.c \
../ChibiOS/os/hal/src/dac.c \
../ChibiOS/os/hal/src/ext.c \
../ChibiOS/os/hal/src/gpt.c \
../ChibiOS/os/hal/src/hal.c \
../ChibiOS/os/hal/src/i2c.c \
../ChibiOS/os/hal/src/icu.c \
../ChibiOS/os/hal/src/mac.c \
../ChibiOS/os/hal/src/mmc_spi.c \
../ChibiOS/os/hal/src/mmcsd.c \
../ChibiOS/os/hal/src/pal.c \
../ChibiOS/os/hal/src/pwm.c \
../ChibiOS/os/hal/src/rtc.c \
../ChibiOS/os/hal/src/sdc.c \
../ChibiOS/os/hal/src/serial.c \
../ChibiOS/os/hal/src/serial_usb.c \
../ChibiOS/os/hal/src/spi.c \
../ChibiOS/os/hal/src/tm.c \
../ChibiOS/os/hal/src/uart.c \
../ChibiOS/os/hal/src/usb.c 

OBJS += \
./ChibiOS/os/hal/src/adc.o \
./ChibiOS/os/hal/src/can.o \
./ChibiOS/os/hal/src/dac.o \
./ChibiOS/os/hal/src/ext.o \
./ChibiOS/os/hal/src/gpt.o \
./ChibiOS/os/hal/src/hal.o \
./ChibiOS/os/hal/src/i2c.o \
./ChibiOS/os/hal/src/icu.o \
./ChibiOS/os/hal/src/mac.o \
./ChibiOS/os/hal/src/mmc_spi.o \
./ChibiOS/os/hal/src/mmcsd.o \
./ChibiOS/os/hal/src/pal.o \
./ChibiOS/os/hal/src/pwm.o \
./ChibiOS/os/hal/src/rtc.o \
./ChibiOS/os/hal/src/sdc.o \
./ChibiOS/os/hal/src/serial.o \
./ChibiOS/os/hal/src/serial_usb.o \
./ChibiOS/os/hal/src/spi.o \
./ChibiOS/os/hal/src/tm.o \
./ChibiOS/os/hal/src/uart.o \
./ChibiOS/os/hal/src/usb.o 

C_DEPS += \
./ChibiOS/os/hal/src/adc.d \
./ChibiOS/os/hal/src/can.d \
./ChibiOS/os/hal/src/dac.d \
./ChibiOS/os/hal/src/ext.d \
./ChibiOS/os/hal/src/gpt.d \
./ChibiOS/os/hal/src/hal.d \
./ChibiOS/os/hal/src/i2c.d \
./ChibiOS/os/hal/src/icu.d \
./ChibiOS/os/hal/src/mac.d \
./ChibiOS/os/hal/src/mmc_spi.d \
./ChibiOS/os/hal/src/mmcsd.d \
./ChibiOS/os/hal/src/pal.d \
./ChibiOS/os/hal/src/pwm.d \
./ChibiOS/os/hal/src/rtc.d \
./ChibiOS/os/hal/src/sdc.d \
./ChibiOS/os/hal/src/serial.d \
./ChibiOS/os/hal/src/serial_usb.d \
./ChibiOS/os/hal/src/spi.d \
./ChibiOS/os/hal/src/tm.d \
./ChibiOS/os/hal/src/uart.d \
./ChibiOS/os/hal/src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/src/%.o: ../ChibiOS/os/hal/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


