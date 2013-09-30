################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/STM32/can_lld.c \
../ChibiOS/os/hal/platforms/STM32/ext_lld.c \
../ChibiOS/os/hal/platforms/STM32/gpt_lld.c \
../ChibiOS/os/hal/platforms/STM32/i2s_lld.c \
../ChibiOS/os/hal/platforms/STM32/icu_lld.c \
../ChibiOS/os/hal/platforms/STM32/mac_lld.c \
../ChibiOS/os/hal/platforms/STM32/pwm_lld.c \
../ChibiOS/os/hal/platforms/STM32/sdc_lld.c 

OBJS += \
./ChibiOS/os/hal/platforms/STM32/can_lld.o \
./ChibiOS/os/hal/platforms/STM32/ext_lld.o \
./ChibiOS/os/hal/platforms/STM32/gpt_lld.o \
./ChibiOS/os/hal/platforms/STM32/i2s_lld.o \
./ChibiOS/os/hal/platforms/STM32/icu_lld.o \
./ChibiOS/os/hal/platforms/STM32/mac_lld.o \
./ChibiOS/os/hal/platforms/STM32/pwm_lld.o \
./ChibiOS/os/hal/platforms/STM32/sdc_lld.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/STM32/can_lld.d \
./ChibiOS/os/hal/platforms/STM32/ext_lld.d \
./ChibiOS/os/hal/platforms/STM32/gpt_lld.d \
./ChibiOS/os/hal/platforms/STM32/i2s_lld.d \
./ChibiOS/os/hal/platforms/STM32/icu_lld.d \
./ChibiOS/os/hal/platforms/STM32/mac_lld.d \
./ChibiOS/os/hal/platforms/STM32/pwm_lld.d \
./ChibiOS/os/hal/platforms/STM32/sdc_lld.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/STM32/%.o: ../ChibiOS/os/hal/platforms/STM32/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


