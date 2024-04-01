#ifndef SYSTICK_H
#define SYSTICK_H


#include "platform_types.h"


#define STK_delay_polling()		STK_start();\
					while ( STK_isExpire() == 0);


typedef void (*STK_CBFptr_t)(void);

void STK_setTime_ms(uint16 num_of_milliseconds);
void STK_enableInterupts();
void STK_disableInterupts();
void STK_clearCountFlag();
void STK_start();
void STK_stop();
void STK_setCallBack(STK_CBFptr_t stkHandlerfunction);
uint8 STK_isExpire();


#endif