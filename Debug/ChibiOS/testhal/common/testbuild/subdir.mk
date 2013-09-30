################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/testhal/common/testbuild/main.c 

OBJS += \
./ChibiOS/testhal/common/testbuild/main.o 

C_DEPS += \
./ChibiOS/testhal/common/testbuild/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/testhal/common/testbuild/%.o: ../ChibiOS/testhal/common/testbuild/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


