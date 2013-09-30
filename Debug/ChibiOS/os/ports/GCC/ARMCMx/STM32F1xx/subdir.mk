################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/ports/GCC/ARMCMx/STM32F1xx/vectors.c 

OBJS += \
./ChibiOS/os/ports/GCC/ARMCMx/STM32F1xx/vectors.o 

C_DEPS += \
./ChibiOS/os/ports/GCC/ARMCMx/STM32F1xx/vectors.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/ports/GCC/ARMCMx/STM32F1xx/%.o: ../ChibiOS/os/ports/GCC/ARMCMx/STM32F1xx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


