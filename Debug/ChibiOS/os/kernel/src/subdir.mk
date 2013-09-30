################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/kernel/src/chcond.c \
../ChibiOS/os/kernel/src/chdebug.c \
../ChibiOS/os/kernel/src/chdynamic.c \
../ChibiOS/os/kernel/src/chevents.c \
../ChibiOS/os/kernel/src/chheap.c \
../ChibiOS/os/kernel/src/chlists.c \
../ChibiOS/os/kernel/src/chmboxes.c \
../ChibiOS/os/kernel/src/chmemcore.c \
../ChibiOS/os/kernel/src/chmempools.c \
../ChibiOS/os/kernel/src/chmsg.c \
../ChibiOS/os/kernel/src/chmtx.c \
../ChibiOS/os/kernel/src/chqueues.c \
../ChibiOS/os/kernel/src/chregistry.c \
../ChibiOS/os/kernel/src/chschd.c \
../ChibiOS/os/kernel/src/chsem.c \
../ChibiOS/os/kernel/src/chsys.c \
../ChibiOS/os/kernel/src/chthreads.c \
../ChibiOS/os/kernel/src/chvt.c 

OBJS += \
./ChibiOS/os/kernel/src/chcond.o \
./ChibiOS/os/kernel/src/chdebug.o \
./ChibiOS/os/kernel/src/chdynamic.o \
./ChibiOS/os/kernel/src/chevents.o \
./ChibiOS/os/kernel/src/chheap.o \
./ChibiOS/os/kernel/src/chlists.o \
./ChibiOS/os/kernel/src/chmboxes.o \
./ChibiOS/os/kernel/src/chmemcore.o \
./ChibiOS/os/kernel/src/chmempools.o \
./ChibiOS/os/kernel/src/chmsg.o \
./ChibiOS/os/kernel/src/chmtx.o \
./ChibiOS/os/kernel/src/chqueues.o \
./ChibiOS/os/kernel/src/chregistry.o \
./ChibiOS/os/kernel/src/chschd.o \
./ChibiOS/os/kernel/src/chsem.o \
./ChibiOS/os/kernel/src/chsys.o \
./ChibiOS/os/kernel/src/chthreads.o \
./ChibiOS/os/kernel/src/chvt.o 

C_DEPS += \
./ChibiOS/os/kernel/src/chcond.d \
./ChibiOS/os/kernel/src/chdebug.d \
./ChibiOS/os/kernel/src/chdynamic.d \
./ChibiOS/os/kernel/src/chevents.d \
./ChibiOS/os/kernel/src/chheap.d \
./ChibiOS/os/kernel/src/chlists.d \
./ChibiOS/os/kernel/src/chmboxes.d \
./ChibiOS/os/kernel/src/chmemcore.d \
./ChibiOS/os/kernel/src/chmempools.d \
./ChibiOS/os/kernel/src/chmsg.d \
./ChibiOS/os/kernel/src/chmtx.d \
./ChibiOS/os/kernel/src/chqueues.d \
./ChibiOS/os/kernel/src/chregistry.d \
./ChibiOS/os/kernel/src/chschd.d \
./ChibiOS/os/kernel/src/chsem.d \
./ChibiOS/os/kernel/src/chsys.d \
./ChibiOS/os/kernel/src/chthreads.d \
./ChibiOS/os/kernel/src/chvt.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/kernel/src/%.o: ../ChibiOS/os/kernel/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


