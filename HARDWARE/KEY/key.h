#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 

#define _KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//��ȡ����0
#define _KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//��ȡ����1
#define _KEY2   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
//��������
void ReadKeyStatus(void);

static void Get_Key_Level(void);
static uint8_t KEY0_ReadPin(void);
static uint8_t KEY1_ReadPin(void);
static uint8_t KEY2_ReadPin(void);
//�ṹ������
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
	uint8_t KEY_SHIELD;		//�������� 0:���� 1:������
	uint8_t KEY_COUNT;		//������������
	uint8_t KEY_LEVEL;		//���⵱ǰIO��ƽ,����1,̧��0
	uint8_t KEY_DOWN_LEVEL;	//����ʱIOʵ�ʵ�ƽ
	uint8_t KEY_STATUS;		//����״̬
	uint8_t KEY_EVENT;		//�����¼�
	uint8_t (*READ_PIN) (void);		//��IO��ƽ����
}KEY_COMPONENTS;

extern KEY_COMPONENTS Key_Buf[KEY_NUM];


u8 tset(void);

