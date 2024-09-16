################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/hal/base.cpp \
../Core/Src/hal/key.cpp \
../Core/Src/hal/oled.cpp \
../Core/Src/hal/system.cpp 

OBJS += \
./Core/Src/hal/base.o \
./Core/Src/hal/key.o \
./Core/Src/hal/oled.o \
./Core/Src/hal/system.o 

CPP_DEPS += \
./Core/Src/hal/base.d \
./Core/Src/hal/key.d \
./Core/Src/hal/oled.d \
./Core/Src/hal/system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/hal/%.o Core/Src/hal/%.su Core/Src/hal/%.cyclo: ../Core/Src/hal/%.cpp Core/Src/hal/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-hal

clean-Core-2f-Src-2f-hal:
	-$(RM) ./Core/Src/hal/base.cyclo ./Core/Src/hal/base.d ./Core/Src/hal/base.o ./Core/Src/hal/base.su ./Core/Src/hal/key.cyclo ./Core/Src/hal/key.d ./Core/Src/hal/key.o ./Core/Src/hal/key.su ./Core/Src/hal/oled.cyclo ./Core/Src/hal/oled.d ./Core/Src/hal/oled.o ./Core/Src/hal/oled.su ./Core/Src/hal/system.cyclo ./Core/Src/hal/system.d ./Core/Src/hal/system.o ./Core/Src/hal/system.su

.PHONY: clean-Core-2f-Src-2f-hal

