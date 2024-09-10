################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/hal/hal_dreamCore/components/hal_config.cpp \
../Core/Src/hal/hal_dreamCore/components/hal_key.cpp \
../Core/Src/hal/hal_dreamCore/components/hal_sys.cpp 

OBJS += \
./Core/Src/hal/hal_dreamCore/components/hal_config.o \
./Core/Src/hal/hal_dreamCore/components/hal_key.o \
./Core/Src/hal/hal_dreamCore/components/hal_sys.o 

CPP_DEPS += \
./Core/Src/hal/hal_dreamCore/components/hal_config.d \
./Core/Src/hal/hal_dreamCore/components/hal_key.d \
./Core/Src/hal/hal_dreamCore/components/hal_sys.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/hal/hal_dreamCore/components/%.o Core/Src/hal/hal_dreamCore/components/%.su Core/Src/hal/hal_dreamCore/components/%.cyclo: ../Core/Src/hal/hal_dreamCore/components/%.cpp Core/Src/hal/hal_dreamCore/components/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-hal-2f-hal_dreamCore-2f-components

clean-Core-2f-Src-2f-hal-2f-hal_dreamCore-2f-components:
	-$(RM) ./Core/Src/hal/hal_dreamCore/components/hal_config.cyclo ./Core/Src/hal/hal_dreamCore/components/hal_config.d ./Core/Src/hal/hal_dreamCore/components/hal_config.o ./Core/Src/hal/hal_dreamCore/components/hal_config.su ./Core/Src/hal/hal_dreamCore/components/hal_key.cyclo ./Core/Src/hal/hal_dreamCore/components/hal_key.d ./Core/Src/hal/hal_dreamCore/components/hal_key.o ./Core/Src/hal/hal_dreamCore/components/hal_key.su ./Core/Src/hal/hal_dreamCore/components/hal_sys.cyclo ./Core/Src/hal/hal_dreamCore/components/hal_sys.d ./Core/Src/hal/hal_dreamCore/components/hal_sys.o ./Core/Src/hal/hal_dreamCore/components/hal_sys.su

.PHONY: clean-Core-2f-Src-2f-hal-2f-hal_dreamCore-2f-components

