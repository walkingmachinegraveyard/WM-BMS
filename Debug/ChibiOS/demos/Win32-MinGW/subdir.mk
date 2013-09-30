################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/demos/Win32-MinGW/main.c 

OBJS += \
./ChibiOS/demos/Win32-MinGW/main.o 

C_DEPS += \
./ChibiOS/demos/Win32-MinGW/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/demos/Win32-MinGW/%.o: ../ChibiOS/demos/Win32-MinGW/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


