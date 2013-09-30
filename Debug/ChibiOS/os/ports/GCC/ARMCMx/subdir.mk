################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/ports/GCC/ARMCMx/chcore.c \
../ChibiOS/os/ports/GCC/ARMCMx/chcore_v6m.c \
../ChibiOS/os/ports/GCC/ARMCMx/chcore_v7m.c \
../ChibiOS/os/ports/GCC/ARMCMx/crt0.c 

OBJS += \
./ChibiOS/os/ports/GCC/ARMCMx/chcore.o \
./ChibiOS/os/ports/GCC/ARMCMx/chcore_v6m.o \
./ChibiOS/os/ports/GCC/ARMCMx/chcore_v7m.o \
./ChibiOS/os/ports/GCC/ARMCMx/crt0.o 

C_DEPS += \
./ChibiOS/os/ports/GCC/ARMCMx/chcore.d \
./ChibiOS/os/ports/GCC/ARMCMx/chcore_v6m.d \
./ChibiOS/os/ports/GCC/ARMCMx/chcore_v7m.d \
./ChibiOS/os/ports/GCC/ARMCMx/crt0.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/ports/GCC/ARMCMx/%.o: ../ChibiOS/os/ports/GCC/ARMCMx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


