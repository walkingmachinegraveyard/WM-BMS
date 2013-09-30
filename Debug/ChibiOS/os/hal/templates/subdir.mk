################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/templates/adc_lld.c \
../ChibiOS/os/hal/templates/can_lld.c \
../ChibiOS/os/hal/templates/ext_lld.c \
../ChibiOS/os/hal/templates/gpt_lld.c \
../ChibiOS/os/hal/templates/hal_lld.c \
../ChibiOS/os/hal/templates/i2c_lld.c \
../ChibiOS/os/hal/templates/icu_lld.c \
../ChibiOS/os/hal/templates/mac_lld.c \
../ChibiOS/os/hal/templates/pal_lld.c \
../ChibiOS/os/hal/templates/pwm_lld.c \
../ChibiOS/os/hal/templates/sdc_lld.c \
../ChibiOS/os/hal/templates/serial_lld.c \
../ChibiOS/os/hal/templates/spi_lld.c \
../ChibiOS/os/hal/templates/uart_lld.c \
../ChibiOS/os/hal/templates/usb_lld.c 

OBJS += \
./ChibiOS/os/hal/templates/adc_lld.o \
./ChibiOS/os/hal/templates/can_lld.o \
./ChibiOS/os/hal/templates/ext_lld.o \
./ChibiOS/os/hal/templates/gpt_lld.o \
./ChibiOS/os/hal/templates/hal_lld.o \
./ChibiOS/os/hal/templates/i2c_lld.o \
./ChibiOS/os/hal/templates/icu_lld.o \
./ChibiOS/os/hal/templates/mac_lld.o \
./ChibiOS/os/hal/templates/pal_lld.o \
./ChibiOS/os/hal/templates/pwm_lld.o \
./ChibiOS/os/hal/templates/sdc_lld.o \
./ChibiOS/os/hal/templates/serial_lld.o \
./ChibiOS/os/hal/templates/spi_lld.o \
./ChibiOS/os/hal/templates/uart_lld.o \
./ChibiOS/os/hal/templates/usb_lld.o 

C_DEPS += \
./ChibiOS/os/hal/templates/adc_lld.d \
./ChibiOS/os/hal/templates/can_lld.d \
./ChibiOS/os/hal/templates/ext_lld.d \
./ChibiOS/os/hal/templates/gpt_lld.d \
./ChibiOS/os/hal/templates/hal_lld.d \
./ChibiOS/os/hal/templates/i2c_lld.d \
./ChibiOS/os/hal/templates/icu_lld.d \
./ChibiOS/os/hal/templates/mac_lld.d \
./ChibiOS/os/hal/templates/pal_lld.d \
./ChibiOS/os/hal/templates/pwm_lld.d \
./ChibiOS/os/hal/templates/sdc_lld.d \
./ChibiOS/os/hal/templates/serial_lld.d \
./ChibiOS/os/hal/templates/spi_lld.d \
./ChibiOS/os/hal/templates/uart_lld.d \
./ChibiOS/os/hal/templates/usb_lld.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/templates/%.o: ../ChibiOS/os/hal/templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


