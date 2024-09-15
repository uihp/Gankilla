################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/hal/hal_dreamCore/memory_pool.cpp 

OBJS += \
./Core/Src/hal/hal_dreamCore/memory_pool.o 

CPP_DEPS += \
./Core/Src/hal/hal_dreamCore/memory_pool.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/hal/hal_dreamCore/%.o Core/Src/hal/hal_dreamCore/%.su Core/Src/hal/hal_dreamCore/%.cyclo: ../Core/Src/hal/hal_dreamCore/%.cpp Core/Src/hal/hal_dreamCore/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-hal-2f-hal_dreamCore

clean-Core-2f-Src-2f-hal-2f-hal_dreamCore:
	-$(RM) ./Core/Src/hal/hal_dreamCore/memory_pool.cyclo ./Core/Src/hal/hal_dreamCore/memory_pool.d ./Core/Src/hal/hal_dreamCore/memory_pool.o ./Core/Src/hal/hal_dreamCore/memory_pool.su

.PHONY: clean-Core-2f-Src-2f-hal-2f-hal_dreamCore

