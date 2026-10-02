#ifndef __JFC_H
#define __JFC_H

#include <stdio.h>

#define USART2_REC_LEN  			24  	//定义最大接收字节数 200

extern uint8_t Serial2_TxPacket[];
extern uint8_t Serial2_RxPacket[];
extern u8 senor2_Flag;
extern u8 USART2_RX_BUF[];     //接收缓冲,最大USART_REC_LEN个字节.
extern u8 USART2_RX_STA;       //接收状态标记	

void JFC_Init(u32 bound);
void Serial2_SendByte(uint8_t Byte);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif
