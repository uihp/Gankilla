#ifndef __OLED_H__
#define __OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---- C ----*/
typedef enum {
  OLED_COLOR_NORMAL = 0, // 正常模式 黑底白字
  OLED_COLOR_REVERSED    // 反色模式 白底黑字
} OLED_ColorMode;

#include "stdint.h"

void OLED_NewFrame();
void OLED_ShowFrame();

void OLED_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);

void OLED_PrintASCIIString(uint8_t x, uint8_t y, char *str, OLED_ColorMode color);
void OLED_PrintString(uint8_t x, uint8_t y, char *str, OLED_ColorMode color);

/*---- C ----*/

#ifdef __cplusplus
}

/*---- Cpp ----*/

/*---- Cpp ----*/

#endif
#endif //ASTRA_CORE_SRC_ASTRA_ASTRA_ROCKET_H_
