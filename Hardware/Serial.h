#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>


#define USART_REC_LEN  			88  	//定义最大接收字节数 200

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];
extern u8 senor_Flag;
extern u8 USART_RX_BUF[];     //接收缓冲,最大USART_REC_LEN个字节.
extern u8 USART_RX_STA;       //接收状态标记	
void Serial_Init(u32 bound);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

void Serial_SendPacket(u8 *Serial_TxPacket,u8 len);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif
