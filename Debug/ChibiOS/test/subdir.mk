################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/test/test.c \
../ChibiOS/test/testbmk.c \
../ChibiOS/test/testdyn.c \
../ChibiOS/test/testevt.c \
../ChibiOS/test/testheap.c \
../ChibiOS/test/testmbox.c \
../ChibiOS/test/testmsg.c \
../ChibiOS/test/testmtx.c \
../ChibiOS/test/testpools.c \
../ChibiOS/test/testqueues.c \
../ChibiOS/test/testsem.c \
../ChibiOS/test/testthd.c 

OBJS += \
./ChibiOS/test/test.o \
./ChibiOS/test/testbmk.o \
./ChibiOS/test/testdyn.o \
./ChibiOS/test/testevt.o \
./ChibiOS/test/testheap.o \
./ChibiOS/test/testmbox.o \
./ChibiOS/test/testmsg.o \
./ChibiOS/test/testmtx.o \
./ChibiOS/test/testpools.o \
./ChibiOS/test/testqueues.o \
./ChibiOS/test/testsem.o \
./ChibiOS/test/testthd.o 

C_DEPS += \
./ChibiOS/test/test.d \
./ChibiOS/test/testbmk.d \
./ChibiOS/test/testdyn.d \
./ChibiOS/test/testevt.d \
./ChibiOS/test/testheap.d \
./ChibiOS/test/testmbox.d \
./ChibiOS/test/testmsg.d \
./ChibiOS/test/testmtx.d \
./ChibiOS/test/testpools.d \
./ChibiOS/test/testqueues.d \
./ChibiOS/test/testsem.d \
./ChibiOS/test/testthd.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/test/%.o: ../ChibiOS/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


