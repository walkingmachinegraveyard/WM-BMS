################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/various/lwip_bindings/lwipthread.c 

OBJS += \
./ChibiOS/os/various/lwip_bindings/lwipthread.o 

C_DEPS += \
./ChibiOS/os/various/lwip_bindings/lwipthread.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/various/lwip_bindings/%.o: ../ChibiOS/os/various/lwip_bindings/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


