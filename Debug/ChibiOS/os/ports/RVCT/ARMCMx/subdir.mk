################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/ports/RVCT/ARMCMx/chcore.c \
../ChibiOS/os/ports/RVCT/ARMCMx/chcore_v6m.c \
../ChibiOS/os/ports/RVCT/ARMCMx/chcore_v7m.c 

OBJS += \
./ChibiOS/os/ports/RVCT/ARMCMx/chcore.o \
./ChibiOS/os/ports/RVCT/ARMCMx/chcore_v6m.o \
./ChibiOS/os/ports/RVCT/ARMCMx/chcore_v7m.o 

C_DEPS += \
./ChibiOS/os/ports/RVCT/ARMCMx/chcore.d \
./ChibiOS/os/ports/RVCT/ARMCMx/chcore_v6m.d \
./ChibiOS/os/ports/RVCT/ARMCMx/chcore_v7m.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/ports/RVCT/ARMCMx/%.o: ../ChibiOS/os/ports/RVCT/ARMCMx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


