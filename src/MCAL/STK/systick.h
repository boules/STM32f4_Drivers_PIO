#ifndef SYSTICK_H
#define SYSTICK_H


#include "platform_types.h"





typedef void (*STK_CBFptr_t)(void);

void STK_setTime_ms(uint16 num_of_milliseconds);
void STK_enableInterupts();
void STK_disableInterupts();
void STK_clearCountFlag();
void STK_start();
void STK_stop();
void STK_setCallBack(STK_CBFptr_t stkHandlerfunction);
uint8 STK_isExpire();


void STK_startITms_SetCBF(uint16 num_of_milliseconds, STK_CBFptr_t stkHandlerfunction);
void STK_setIT_time_CBF(uint16 num_of_milliseconds, STK_CBFptr_t stkHandlerfunction);




#define STK_delay_ms_polling(mstime)		STK_stop();\
											STK_setTime_ms(mstime);\
											STK_start();\
											while ( STK_isExpire() == 0);

#endif