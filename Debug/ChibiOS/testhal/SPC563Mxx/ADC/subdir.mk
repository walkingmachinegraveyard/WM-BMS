################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChibiOS/testhal/SPC563Mxx/ADC/adc_cfg.c \
../ChibiOS/testhal/SPC563Mxx/ADC/main.c 

OBJS += \
./ChibiOS/testhal/SPC563Mxx/ADC/adc_cfg.o \
./ChibiOS/testhal/SPC563Mxx/ADC/main.o 

C_DEPS += \
./ChibiOS/testhal/SPC563Mxx/ADC/adc_cfg.d \
./ChibiOS/testhal/SPC563Mxx/ADC/main.d 


# Each subdirectory must supply rules for building sources it contributes
ChibiOS/testhal/SPC563Mxx/ADC/%.o: ../ChibiOS/testhal/SPC563Mxx/ADC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/francois/BMS_PROGProto/bms/ChibiOS" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


