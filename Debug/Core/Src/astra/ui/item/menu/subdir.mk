################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/astra/ui/item/menu/menu.cpp 

OBJS += \
./Core/Src/astra/ui/item/menu/menu.o 

CPP_DEPS += \
./Core/Src/astra/ui/item/menu/menu.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/astra/ui/item/menu/%.o Core/Src/astra/ui/item/menu/%.su Core/Src/astra/ui/item/menu/%.cyclo: ../Core/Src/astra/ui/item/menu/%.cpp Core/Src/astra/ui/item/menu/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Oz -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-astra-2f-ui-2f-item-2f-menu

clean-Core-2f-Src-2f-astra-2f-ui-2f-item-2f-menu:
	-$(RM) ./Core/Src/astra/ui/item/menu/menu.cyclo ./Core/Src/astra/ui/item/menu/menu.d ./Core/Src/astra/ui/item/menu/menu.o ./Core/Src/astra/ui/item/menu/menu.su

.PHONY: clean-Core-2f-Src-2f-astra-2f-ui-2f-item-2f-menu

