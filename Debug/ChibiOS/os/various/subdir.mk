################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/various/chprintf.c \
../ChibiOS/os/various/chrtclib.c \
../ChibiOS/os/various/evtimer.c \
../ChibiOS/os/various/memstreams.c \
../ChibiOS/os/various/shell.c \
../ChibiOS/os/various/syscalls.c 

OBJS += \
./ChibiOS/os/various/chprintf.o \
./ChibiOS/os/various/chrtclib.o \
./ChibiOS/os/various/evtimer.o \
./ChibiOS/os/various/memstreams.o \
./ChibiOS/os/various/shell.o \
./ChibiOS/os/various/syscalls.o 

C_DEPS += \
./ChibiOS/os/various/chprintf.d \
./ChibiOS/os/various/chrtclib.d \
./ChibiOS/os/various/evtimer.d \
./ChibiOS/os/various/memstreams.d \
./ChibiOS/os/various/shell.d \
./ChibiOS/os/various/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/various/%.o: ../ChibiOS/os/various/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


