################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/testhal/STM32F0xx/ADC/main.c 

OBJS += \
./ChibiOS/testhal/STM32F0xx/ADC/main.o 

C_DEPS += \
./ChibiOS/testhal/STM32F0xx/ADC/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/testhal/STM32F0xx/ADC/%.o: ../ChibiOS/testhal/STM32F0xx/ADC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


