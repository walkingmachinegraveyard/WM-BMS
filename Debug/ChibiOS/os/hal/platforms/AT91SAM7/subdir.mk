################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/AT91SAM7/adc_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/at91sam7_mii.c \
../ChibiOS/os/hal/platforms/AT91SAM7/ext_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/hal_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/i2c_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/mac_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/pal_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/pwm_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/serial_lld.c \
../ChibiOS/os/hal/platforms/AT91SAM7/spi_lld.c 

OBJS += \
./ChibiOS/os/hal/platforms/AT91SAM7/adc_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/at91sam7_mii.o \
./ChibiOS/os/hal/platforms/AT91SAM7/ext_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/hal_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/i2c_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/mac_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/pal_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/pwm_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/serial_lld.o \
./ChibiOS/os/hal/platforms/AT91SAM7/spi_lld.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/AT91SAM7/adc_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/at91sam7_mii.d \
./ChibiOS/os/hal/platforms/AT91SAM7/ext_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/hal_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/i2c_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/mac_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/pal_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/pwm_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/serial_lld.d \
./ChibiOS/os/hal/platforms/AT91SAM7/spi_lld.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/AT91SAM7/%.o: ../ChibiOS/os/hal/platforms/AT91SAM7/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


