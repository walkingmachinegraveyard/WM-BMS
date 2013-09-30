################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/various/fatfs_bindings/fatfs_diskio.c \
../ChibiOS/os/various/fatfs_bindings/fatfs_syscall.c 

OBJS += \
./ChibiOS/os/various/fatfs_bindings/fatfs_diskio.o \
./ChibiOS/os/various/fatfs_bindings/fatfs_syscall.o 

C_DEPS += \
./ChibiOS/os/various/fatfs_bindings/fatfs_diskio.d \
./ChibiOS/os/various/fatfs_bindings/fatfs_syscall.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/various/fatfs_bindings/%.o: ../ChibiOS/os/various/fatfs_bindings/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


