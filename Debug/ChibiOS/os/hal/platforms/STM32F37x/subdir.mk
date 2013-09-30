################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/os/hal/platforms/STM32F37x/adc_lld.c \
../ChibiOS/os/hal/platforms/STM32F37x/ext_lld_isr.c \
../ChibiOS/os/hal/platforms/STM32F37x/hal_lld.c \
../ChibiOS/os/hal/platforms/STM32F37x/stm32_dma.c 

OBJS += \
./ChibiOS/os/hal/platforms/STM32F37x/adc_lld.o \
./ChibiOS/os/hal/platforms/STM32F37x/ext_lld_isr.o \
./ChibiOS/os/hal/platforms/STM32F37x/hal_lld.o \
./ChibiOS/os/hal/platforms/STM32F37x/stm32_dma.o 

C_DEPS += \
./ChibiOS/os/hal/platforms/STM32F37x/adc_lld.d \
./ChibiOS/os/hal/platforms/STM32F37x/ext_lld_isr.d \
./ChibiOS/os/hal/platforms/STM32F37x/hal_lld.d \
./ChibiOS/os/hal/platforms/STM32F37x/stm32_dma.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/os/hal/platforms/STM32F37x/%.o: ../ChibiOS/os/hal/platforms/STM32F37x/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


