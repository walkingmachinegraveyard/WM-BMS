################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/boards/OLIMEX_AVR_CAN/board.c 

OBJS += \
./ChibiOS/boards/OLIMEX_AVR_CAN/board.o 

C_DEPS += \
./ChibiOS/boards/OLIMEX_AVR_CAN/board.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/boards/OLIMEX_AVR_CAN/%.o: ../ChibiOS/boards/OLIMEX_AVR_CAN/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


