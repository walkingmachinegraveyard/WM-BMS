################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/demos/AVR-ATmega128-GCC/lcd.c \
../ChibiOS/demos/AVR-ATmega128-GCC/main.c 

OBJS += \
./ChibiOS/demos/AVR-ATmega128-GCC/lcd.o \
./ChibiOS/demos/AVR-ATmega128-GCC/main.o 

C_DEPS += \
./ChibiOS/demos/AVR-ATmega128-GCC/lcd.d \
./ChibiOS/demos/AVR-ATmega128-GCC/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/demos/AVR-ATmega128-GCC/%.o: ../ChibiOS/demos/AVR-ATmega128-GCC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


