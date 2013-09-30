################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/LPC214x/hal_lld.c \
../ChibiOS/os/hal/platforms/LPC214x/pal_lld.c \
../ChibiOS/os/hal/platforms/LPC214x/serial_lld.c \
../ChibiOS/os/hal/platforms/LPC214x/spi_lld.c \
../ChibiOS/os/hal/platforms/LPC214x/vic.c 

OBJS += \
./ChibiOS/os/hal/platforms/LPC214x/hal_lld.o \
./ChibiOS/os/hal/platforms/LPC214x/pal_lld.o \
./ChibiOS/os/hal/platforms/LPC214x/serial_lld.o \
./ChibiOS/os/hal/platforms/LPC214x/spi_lld.o \
./ChibiOS/os/hal/platforms/LPC214x/vic.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/LPC214x/hal_lld.d \
./ChibiOS/os/hal/platforms/LPC214x/pal_lld.d \
./ChibiOS/os/hal/platforms/LPC214x/serial_lld.d \
./ChibiOS/os/hal/platforms/LPC214x/spi_lld.d \
./ChibiOS/os/hal/platforms/LPC214x/vic.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/LPC214x/%.o: ../ChibiOS/os/hal/platforms/LPC214x/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


