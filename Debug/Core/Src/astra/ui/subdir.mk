################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/astra/ui/launcher.cpp 

OBJS += \
./Core/Src/astra/ui/launcher.o 

CPP_DEPS += \
./Core/Src/astra/ui/launcher.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/astra/ui/%.o Core/Src/astra/ui/%.su Core/Src/astra/ui/%.cyclo: ../Core/Src/astra/ui/%.cpp Core/Src/astra/ui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-astra-2f-ui

clean-Core-2f-Src-2f-astra-2f-ui:
	-$(RM) ./Core/Src/astra/ui/launcher.cyclo ./Core/Src/astra/ui/launcher.d ./Core/Src/astra/ui/launcher.o ./Core/Src/astra/ui/launcher.su

.PHONY: clean-Core-2f-Src-2f-astra-2f-ui

