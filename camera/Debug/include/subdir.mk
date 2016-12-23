################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/zzuummaa/workspace/RosCanSat/telemetry/include/pipe.c \
/home/zzuummaa/workspace/RosCanSat/telemetry/include/structs.c 

OBJS += \
./include/pipe.o \
./include/structs.o 

C_DEPS += \
./include/pipe.d \
./include/structs.d 


# Each subdirectory must supply rules for building sources it contributes
include/pipe.o: /home/zzuummaa/workspace/RosCanSat/telemetry/include/pipe.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/zzuummaa/workspace/RosCanSat/telemetry/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/structs.o: /home/zzuummaa/workspace/RosCanSat/telemetry/include/structs.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/zzuummaa/workspace/RosCanSat/telemetry/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


