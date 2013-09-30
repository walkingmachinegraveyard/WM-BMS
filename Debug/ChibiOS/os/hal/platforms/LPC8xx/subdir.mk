################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/LPC8xx/ext_lld.c \
../ChibiOS/os/hal/platforms/LPC8xx/ext_lld_isr.c \
../ChibiOS/os/hal/platforms/LPC8xx/gpt_lld.c \
../ChibiOS/os/hal/platforms/LPC8xx/hal_lld.c \
../ChibiOS/os/hal/platforms/LPC8xx/pal_lld.c \
../ChibiOS/os/hal/platforms/LPC8xx/serial_lld.c \
../ChibiOS/os/hal/platforms/LPC8xx/spi_lld.c 

OBJS += \
./ChibiOS/os/hal/platforms/LPC8xx/ext_lld.o \
./ChibiOS/os/hal/platforms/LPC8xx/ext_lld_isr.o \
./ChibiOS/os/hal/platforms/LPC8xx/gpt_lld.o \
./ChibiOS/os/hal/platforms/LPC8xx/hal_lld.o \
./ChibiOS/os/hal/platforms/LPC8xx/pal_lld.o \
./ChibiOS/os/hal/platforms/LPC8xx/serial_lld.o \
./ChibiOS/os/hal/platforms/LPC8xx/spi_lld.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/LPC8xx/ext_lld.d \
./ChibiOS/os/hal/platforms/LPC8xx/ext_lld_isr.d \
./ChibiOS/os/hal/platforms/LPC8xx/gpt_lld.d \
./ChibiOS/os/hal/platforms/LPC8xx/hal_lld.d \
./ChibiOS/os/hal/platforms/LPC8xx/pal_lld.d \
./ChibiOS/os/hal/platforms/LPC8xx/serial_lld.d \
./ChibiOS/os/hal/platforms/LPC8xx/spi_lld.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/LPC8xx/%.o: ../ChibiOS/os/hal/platforms/LPC8xx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


