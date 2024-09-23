#include "components.h"
#include "main.h"

void MixinHAL::_delay(unsigned long _mill) {
  HAL_Delay(_mill);
}

unsigned long MixinHAL::_millis() {
  return HAL_GetTick();
}

unsigned long MixinHAL::_getTick() {
  return (uwTick * 1000 + (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock/1000000U));
}

unsigned long MixinHAL::_getRandomSeed() {
  static uint32_t seed = 0;
  HAL_ADC_Start(&hadc1); // 开启ADC1
  HAL_ADC_PollForConversion(&hadc1, 50); // 表示等待转换完成
  if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC)) {
    seed = HAL_ADC_GetValue(&hadc1); // 读取ADC转换数据
  }
  return seed;
}
