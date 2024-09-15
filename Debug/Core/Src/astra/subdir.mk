################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/astra/astra_logo.cpp \
../Core/Src/astra/astra_rocket.cpp 

OBJS += \
./Core/Src/astra/astra_logo.o \
./Core/Src/astra/astra_rocket.o 

CPP_DEPS += \
./Core/Src/astra/astra_logo.d \
./Core/Src/astra/astra_rocket.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/astra/%.o Core/Src/astra/%.su Core/Src/astra/%.cyclo: ../Core/Src/astra/%.cpp Core/Src/astra/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-astra

clean-Core-2f-Src-2f-astra:
	-$(RM) ./Core/Src/astra/astra_logo.cyclo ./Core/Src/astra/astra_logo.d ./Core/Src/astra/astra_logo.o ./Core/Src/astra/astra_logo.su ./Core/Src/astra/astra_rocket.cyclo ./Core/Src/astra/astra_rocket.d ./Core/Src/astra/astra_rocket.o ./Core/Src/astra/astra_rocket.su

.PHONY: clean-Core-2f-Src-2f-astra

