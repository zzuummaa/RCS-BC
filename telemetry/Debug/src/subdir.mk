################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/workspace/camera/src/camera.cpp 

OBJS += \
./src/camera.o 

CPP_DEPS += \
./src/camera.d 


# Each subdirectory must supply rules for building sources it contributes
src/camera.o: /home/zzuummaa/workspace/camera/src/camera.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/zzuummaa/workspace/camera/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


