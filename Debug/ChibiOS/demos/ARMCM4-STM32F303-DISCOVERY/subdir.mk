################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/demos/ARMCM4-STM32F303-DISCOVERY/main.c 

OBJS += \
./ChibiOS/demos/ARMCM4-STM32F303-DISCOVERY/main.o 

C_DEPS += \
./ChibiOS/demos/ARMCM4-STM32F303-DISCOVERY/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/demos/ARMCM4-STM32F303-DISCOVERY/%.o: ../ChibiOS/demos/ARMCM4-STM32F303-DISCOVERY/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


