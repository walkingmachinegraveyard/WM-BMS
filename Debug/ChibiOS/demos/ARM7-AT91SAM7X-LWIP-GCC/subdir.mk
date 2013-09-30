################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/demos/ARM7-AT91SAM7X-LWIP-GCC/main.c 

OBJS += \
./ChibiOS/demos/ARM7-AT91SAM7X-LWIP-GCC/main.o 

C_DEPS += \
./ChibiOS/demos/ARM7-AT91SAM7X-LWIP-GCC/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/demos/ARM7-AT91SAM7X-LWIP-GCC/%.o: ../ChibiOS/demos/ARM7-AT91SAM7X-LWIP-GCC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


