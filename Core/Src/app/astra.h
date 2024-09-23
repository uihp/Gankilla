//
// Created by Fir on 2024/3/7 007.
// 此文件的作用是引导STM32进入astra UI 基于C++
// this file is used to guide STM32 into astra UI based on C++
//

#ifndef APP_ASTRA__H_
#define APP_ASTRA__H_

#ifdef __cplusplus
extern "C" {
#endif

/*---- C ----*/

void astraCoreInit(void);
void astraCoreUpdate(void);
void astraCoreDestroy(void);

/*---- C ----*/

#ifdef __cplusplus
}

/*---- Cpp ----*/
#include "../astra/launcher.h"
#include "../hal/components.h"

/*---- Cpp ----*/

#endif
#endif // APP_ASTRA__H_
