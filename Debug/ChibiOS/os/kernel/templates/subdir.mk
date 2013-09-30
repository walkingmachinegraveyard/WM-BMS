################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/kernel/templates/chcore.c 

OBJS += \
./ChibiOS/os/kernel/templates/chcore.o 

C_DEPS += \
./ChibiOS/os/kernel/templates/chcore.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/kernel/templates/%.o: ../ChibiOS/os/kernel/templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


