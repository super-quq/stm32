#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_control(void);

#define KEY_ON	1
#define KEY_OFF	0

#define key1_down 	   PAin(0)


#endif


