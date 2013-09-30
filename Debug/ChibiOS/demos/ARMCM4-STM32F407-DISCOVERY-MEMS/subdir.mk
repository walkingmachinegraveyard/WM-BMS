################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/main.c \
../ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/usbcfg.c 

OBJS += \
./ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/main.o \
./ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/usbcfg.o 

C_DEPS += \
./ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/main.d \
./ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/usbcfg.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/%.o: ../ChibiOS/demos/ARMCM4-STM32F407-DISCOVERY-MEMS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


