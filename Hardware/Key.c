#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"
#include "JFC.h"
#include "Beep.h"

/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOA的时钟
	 
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;         //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将引脚初始化为上拉输入

}


/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)			//读PB4输入寄存器的状态，如果为0，则代表按键2按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 1;												//置键码为1
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)			//读PB5输入寄存器的状态，如果为0，则代表按键3按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 2;												//置键码为2
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)			//读PB8输入寄存器的状态，如果为0，则代表按键4按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 3;												//置键码为3
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)			//读PB9输入寄存器的状态，如果为0，则代表按键5按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 4;												//置键码为4
	}
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}

extern u8 KeyNum,mode,hrH,spo2H;
extern u8 Key,A;
extern u8 send2_flag;

void Key_control(void)
{
	 KeyNum=Key_GetNum();
	
	 switch(KeyNum)
	 {
		 case 1:
				 if(send2_flag==0)
				 {
					 Serial2_SendByte(0x24);   //发送模块初始化数据
					 send2_flag=1;             //接收数据位置1
					}
				 else
				 {
					 Serial2_SendByte(0x2A);   //发送模块关闭数据
					 send2_flag=0;
					}
     break;

		 case 4:
					  OLED_Clear(); 
				    mode=0;
		        Beep_off();
		 break;
		
		 case 3:
				 if(mode==0)
				 { 
					 OLED_Clear();
					 mode=1;          //设置hrH阈值
					
					}
				 else if(mode==1)hrH+=5;  //按一下-5
				 else if(mode==2)spo2H+=1;
		 break;
					
		 case 2:
				 if(mode==0)
				 { 
					 OLED_Clear();
					 mode=2;          //设置spo2H阈值
					}
				 else if(mode==1)hrH-=5;  //按一下-5
				 else if(mode==2)spo2H-=1;
		 break;
		 default:break;
	 }
}

