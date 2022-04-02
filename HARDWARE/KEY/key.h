#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 

#define _KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//读取按键0
#define _KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键1
#define _KEY2   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
//函数声明
void ReadKeyStatus(void);

static void Get_Key_Level(void);
static uint8_t KEY0_ReadPin(void);
static uint8_t KEY1_ReadPin(void);
static uint8_t KEY2_ReadPin(void);
//结构体声明
typedef enum _KEY_STATUS_LIST
{
	KEY_NULL = 0x00,
	KEY_SURE = 0x01,
	KEY_UP   = 0x02,
	KEY_DOWN = 0x04,
	KEY_LONG = 0x08,
	KEY_SUPER_LONG = 0x10,
}KEY_STATUS_LIST;

typedef enum _KEY_LIST
{
	KEY0,
	KEY1,
	KEY2,
	KEY_NUM,
}KEY_LIST;

typedef struct _KEY_COMPONENTS
{
	uint8_t KEY_SHIELD;		//按键屏蔽 0:屏蔽 1:不屏蔽
	uint8_t KEY_COUNT;		//按键长按计数
	uint8_t KEY_LEVEL;		//虚拟当前IO电平,按下1,抬起0
	uint8_t KEY_DOWN_LEVEL;	//按下时IO实际电平
	uint8_t KEY_STATUS;		//按键状态
	uint8_t KEY_EVENT;		//按键事件
	uint8_t (*READ_PIN) (void);		//读IO电平函数
}KEY_COMPONENTS;

extern KEY_COMPONENTS Key_Buf[KEY_NUM];


u8 tset(void);

