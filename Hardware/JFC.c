#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "JFC.h"

uint8_t Serial2_TxPacket[100];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
uint8_t Serial2_RxPacket[88];				//定义接收数据包数组
uint8_t Serial2_RxFlag;					//定义接收数据包标志位
uint8_t Serial2_RxData;		//定义串口接收的数据变量
u8 senor2_Flag=0;
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_STA=0;       //接收状态标记	

/**
  * 函    数：串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void JFC_Init(u32 bound)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;    //PA2 Tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA2引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;        //PA3 Rx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA3引脚初始化为上拉输入
	
	/*USART初始化*/
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = bound;				//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(USART2, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
	/*中断输出配置*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*USART使能*/
	USART_Cmd(USART2, ENABLE);								//使能USART1，串口开始运行
}

void Serial2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

void USART2_IRQHandler(void) 
{
	u8 Res;
	static u8 flag;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			Res =USART_ReceiveData(USART2);						//(USART1->DR);	//读取接收到的数据
			if (Res==(0xff)) 									//当数据包的首字节为0xff,则继续接收,否则丢弃
			{
				flag = 1;
			 }
			if(flag)
			{
				USART2_RX_BUF[USART2_RX_STA]=Res;					//每次进中断在这里赋值给数组
				USART2_RX_STA++;																		
			 }
			if(USART2_RX_STA>(USART2_REC_LEN-1))					//达到88字节时,回到数组第1位
			{
				USART2_RX_STA=0;
				flag= 0;
				senor2_Flag=1;
			 }
		   		 
    } 

}
