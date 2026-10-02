#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <stdio.h>
#include "Key.h"
#include "JFC.h"
#include "Beep.h"

u8 Key,A=0;
u8 send2_flag=0;  //串口2
u8 send_buf[20];
u8 hrH=100,spo2H=90,mode,KeyNum;
float temp;

int main(void)
{
	Beep_Init();
	OLED_Init();
	OLED_Show();  //屏幕显示内容
	Key_Init();
	Serial_Init(38400); //串口1,38400
	JFC_Init(9600);   //心率检测芯片固定波特率,串口2
	
	while (1)
	{
     OLED_Show();
		 Key_control();
		
		 if(mode==0)
		 {
			 if(senor2_Flag==1)    //如果接收88个字节即显示数据
			 {
					senor2_Flag=0;
				 if(USART2_RX_BUF[2]>100)                    
						OLED_ShowNum(1,6,USART2_RX_BUF[2],3);     //显示心率
				 else
				 {  
						OLED_ShowString(1,8," ");
						OLED_ShowNum(1,6,USART2_RX_BUF[2],2);
				 }
				 OLED_ShowNum(2,6,USART2_RX_BUF[3],2);        //显示血氧
				 
				 if(USART2_RX_BUF[5]>100)
						OLED_ShowNum(3,6,USART2_RX_BUF[5],3);     //显示高压
				 else
				 {  
						OLED_ShowString(3,8," ");
						OLED_ShowNum(3,6,USART2_RX_BUF[5],2);
					}
				 if(USART2_RX_BUF[6]>100)                    
						OLED_ShowNum(4,6,USART2_RX_BUF[6],3);     //显示低压
				 else
				 { 
						OLED_ShowString(4,8," ");
						OLED_ShowNum(4,6,USART2_RX_BUF[6],2);
				 }
				 
				 temp=((float)(USART2_RX_BUF[12]*100+USART2_RX_BUF[13]))/100;  		//体温换算成小数类型
				 OLED_ShowFloat(4,14,temp,1);
				 
				 if((USART2_RX_BUF[2]>=hrH)||(USART2_RX_BUF[3]<=spo2H&&USART2_RX_BUF[3]>=85))Beep_on(); //蜂鸣器响声条件
				 else Beep_off();
				 
				 send_buf[0] = USART2_RX_BUF[2];
				 send_buf[1] = USART2_RX_BUF[3];
				 send_buf[2] = USART2_RX_BUF[5];
				 send_buf[3] = USART2_RX_BUF[6];
				 send_buf[4] = USART2_RX_BUF[12];
				 send_buf[5] = USART2_RX_BUF[13];
				 Serial_SendPacket(&send_buf[0],6);        //以数组形式发送四个数据到上位机
			 } 
		  }
		 
	 }
}
