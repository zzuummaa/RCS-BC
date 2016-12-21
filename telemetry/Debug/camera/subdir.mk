################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/zzuummaa/workspace/camera/camera.cpp 

OBJS += \
./camera/camera.o 

CPP_DEPS += \
./camera/camera.d 


# Each subdirectory must supply rules for building sources it contributes
camera/camera.o: /home/zzuummaa/workspace/camera/camera.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/zzuummaa/workspace/camera" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


