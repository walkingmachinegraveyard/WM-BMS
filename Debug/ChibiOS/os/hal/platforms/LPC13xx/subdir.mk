################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/LPC13xx/gpt_lld.c \
../ChibiOS/os/hal/platforms/LPC13xx/hal_lld.c \
../ChibiOS/os/hal/platforms/LPC13xx/pal_lld.c \
../ChibiOS/os/hal/platforms/LPC13xx/serial_lld.c \
../ChibiOS/os/hal/platforms/LPC13xx/spi_lld.c 

OBJS += \
./ChibiOS/os/hal/platforms/LPC13xx/gpt_lld.o \
./ChibiOS/os/hal/platforms/LPC13xx/hal_lld.o \
./ChibiOS/os/hal/platforms/LPC13xx/pal_lld.o \
./ChibiOS/os/hal/platforms/LPC13xx/serial_lld.o \
./ChibiOS/os/hal/platforms/LPC13xx/spi_lld.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/LPC13xx/gpt_lld.d \
./ChibiOS/os/hal/platforms/LPC13xx/hal_lld.d \
./ChibiOS/os/hal/platforms/LPC13xx/pal_lld.d \
./ChibiOS/os/hal/platforms/LPC13xx/serial_lld.d \
./ChibiOS/os/hal/platforms/LPC13xx/spi_lld.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/LPC13xx/%.o: ../ChibiOS/os/hal/platforms/LPC13xx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


