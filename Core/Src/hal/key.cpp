#include "main.h"
#include "components.h"

bool MixinHAL::_getKey(key::KEY_INDEX _keyIndex) {
  if (_keyIndex == key::KEY_0) return !HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
  if (_keyIndex == key::KEY_1) return !HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
  if (_keyIndex == key::KEY_2) return !HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
  if (_keyIndex == key::KEY_3) return !HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
  return false;
}
