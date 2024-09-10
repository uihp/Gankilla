################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/hal/hal_dreamCore/components/oled/hal_oled.cpp 

OBJS += \
./Core/Src/hal/hal_dreamCore/components/oled/hal_oled.o 

CPP_DEPS += \
./Core/Src/hal/hal_dreamCore/components/oled/hal_oled.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/hal/hal_dreamCore/components/oled/%.o Core/Src/hal/hal_dreamCore/components/oled/%.su Core/Src/hal/hal_dreamCore/components/oled/%.cyclo: ../Core/Src/hal/hal_dreamCore/components/oled/%.cpp Core/Src/hal/hal_dreamCore/components/oled/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-hal-2f-hal_dreamCore-2f-components-2f-oled

clean-Core-2f-Src-2f-hal-2f-hal_dreamCore-2f-components-2f-oled:
	-$(RM) ./Core/Src/hal/hal_dreamCore/components/oled/hal_oled.cyclo ./Core/Src/hal/hal_dreamCore/components/oled/hal_oled.d ./Core/Src/hal/hal_dreamCore/components/oled/hal_oled.o ./Core/Src/hal/hal_dreamCore/components/oled/hal_oled.su

.PHONY: clean-Core-2f-Src-2f-hal-2f-hal_dreamCore-2f-components-2f-oled

