#include "../hal_dreamCore.h"
#include "main.h"

void HALDreamCore::_delay(unsigned long _mill) {
  HAL_Delay(_mill);
}

unsigned long HALDreamCore::_millis() {
  return HAL_GetTick();
}

unsigned long HALDreamCore::_getTick() {
  return (uwTick * 1000 + (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock/1000000U));
}
