################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../software/src/acs.c \
../software/src/ad72.c \
../software/src/battery.c \
../software/src/cell_mon.c \
../software/src/console.c \
../software/src/main.c 

OBJS += \
./software/src/acs.o \
./software/src/ad72.o \
./software/src/battery.o \
./software/src/cell_mon.o \
./software/src/console.o \
./software/src/main.o 

C_DEPS += \
./software/src/acs.d \
./software/src/ad72.d \
./software/src/battery.d \
./software/src/cell_mon.d \
./software/src/console.d \
./software/src/main.d 


# Each subdirectory must supply rules for building sources it contributes
software/src/%.o: ../software/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


