################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/astra/app/astra_app.cpp 

OBJS += \
./Core/Src/astra/app/astra_app.o 

CPP_DEPS += \
./Core/Src/astra/app/astra_app.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/astra/app/%.o Core/Src/astra/app/%.su Core/Src/astra/app/%.cyclo: ../Core/Src/astra/app/%.cpp Core/Src/astra/app/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-astra-2f-app

clean-Core-2f-Src-2f-astra-2f-app:
	-$(RM) ./Core/Src/astra/app/astra_app.cyclo ./Core/Src/astra/app/astra_app.d ./Core/Src/astra/app/astra_app.o ./Core/Src/astra/app/astra_app.su

.PHONY: clean-Core-2f-Src-2f-astra-2f-app

