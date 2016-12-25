################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/workspace/RosCanSat/telemetry/include/pipe.cpp \
/home/zzuummaa/workspace/RosCanSat/telemetry/include/structs.cpp 

OBJS += \
./include/pipe.o \
./include/structs.o 

CPP_DEPS += \
./include/pipe.d \
./include/structs.d 


# Each subdirectory must supply rules for building sources it contributes
include/pipe.o: /home/zzuummaa/workspace/RosCanSat/telemetry/include/pipe.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/zzuummaa/workspace/RosCanSat/telemetry/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/structs.o: /home/zzuummaa/workspace/RosCanSat/telemetry/include/structs.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/zzuummaa/workspace/RosCanSat/telemetry/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


