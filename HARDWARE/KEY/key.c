#include "key.h"
#define uint8_t unsigned char
#define KEY_LONG_DOWN_DELAY 30 
#define KEY_SUPER_LONG_DOWN_DELAY 100
/*�ٰ������� 0:���� 1:�����Ρ��ڰ������������������⵱ǰIO��ƽ,����1,̧��0
�ܰ���ʱIOʵ�ʵ�ƽ���ݰ���״̬���ް����¼����߶�IO��ƽ����
*/

KEY_COMPONENTS Key_Buf[KEY_NUM] = 
{
	{1,0,0,0,KEY_NULL,KEY_NULL,KEY0_ReadPin},
	{1,0,0,0,KEY_NULL,KEY_NULL,KEY1_ReadPin},
	{1,0,0,1,KEY_NULL,KEY_NULL,KEY2_ReadPin},
	
};

/*��WK_UP���ǹ������ӷ���������Ҫ��������ť���µ�ʱ���Ǹߵ�ƽ���ɿ��ǵ͵�ƽ��
��KEY0��KEY1���ǹ������ⷨ��������Ҫ��������ť���µ�ʱ���ǵ͵�ƽ�����ɿ���ʱ���Ǹߵ�ƽ��*/
static uint8_t KEY0_ReadPin(void)
{
	return _KEY0;
}
static uint8_t KEY1_ReadPin(void)
{
	return _KEY1;
}

static uint8_t KEY2_ReadPin(void)
{
	return _KEY2;
}

static void Get_Key_Level(void)
{
	uint8_t i;
	for(i = 0; i < KEY_NUM; i++)
	{
		if(Key_Buf[i].KEY_SHIELD == 0)
		continue;
		if (Key_Buf[i].READ_PIN() == Key_Buf[i].KEY_DOWN_LEVEL) Key_Buf[i].KEY_LEVEL = 1;
		else Key_Buf[i].KEY_LEVEL = 0;		
	}
}

void KEY_Init(void)//������ʼ��
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
	
} 


// -------------------------------����״̬��
void ReadKeyStatus(void)
{
	uint8_t i;
	Get_Key_Level();
	for ( i = 0; i < KEY_NUM; i++)
	{
		switch (Key_Buf[i].KEY_STATUS)//�а�������
		{	//״̬0������û�а���
			case KEY_NULL:
				if (Key_Buf[i].KEY_LEVEL == 1) //�����������а�������
				{
					Key_Buf[i].KEY_STATUS = KEY_SURE; //ת��״̬1
					Key_Buf[i].KEY_EVENT = KEY_NULL; //���¼�
				}
				else
				{
					Key_Buf[i].KEY_STATUS = KEY_NULL;//δ�����¼�����������ԭ״̬
				}
				break;
			//״̬1����������ȷ��	
			case KEY_SURE:
				if (Key_Buf[i].KEY_LEVEL == 1)//ȷ�ϰ�����Ȼ���ڰ���״̬
				{
					Key_Buf[i].KEY_EVENT = KEY_DOWN;//�����¼�
					Key_Buf[i].KEY_STATUS = KEY_DOWN; //ת��״̬2
					Key_Buf[i].KEY_COUNT = 0; //����������
				}
				else
				{
					Key_Buf[i].KEY_STATUS = KEY_NULL;//ת��״̬0
					Key_Buf[i].KEY_EVENT = KEY_NULL;//���¼�
				}
				break;
				//״̬2����������
			case KEY_DOWN:
				if (Key_Buf[i].KEY_LEVEL != 1) //�����ͷţ��˿ڸߵ�ƽ
				{
					Key_Buf[i].KEY_EVENT = KEY_UP; //�ɿ��¼�
					Key_Buf[i].KEY_STATUS = KEY_NULL; //ת��״̬0
				}
				else if ((Key_Buf[i].KEY_LEVEL == 1) && (++Key_Buf[i].KEY_COUNT >= KEY_LONG_DOWN_DELAY)) //����KEY_LONG_DOWN_DELAYû���ͷ�
				{
					Key_Buf[i].KEY_EVENT = KEY_LONG;//�����¼�
					Key_Buf[i].KEY_STATUS = KEY_LONG; //תΪ״̬3
					Key_Buf[i].KEY_COUNT = 0;//����������
				}
				else
				{
					Key_Buf[i].KEY_EVENT = KEY_NULL;//���¼�
				}
				break;
				//״̬3����������
			case KEY_LONG:
				if (Key_Buf[i].KEY_LEVEL != 1) //�����ͷţ��˿ڸߵ�ƽ
				{
					Key_Buf[i].KEY_EVENT = KEY_UP;//�ɿ��¼�
					Key_Buf[i].KEY_STATUS = KEY_NULL; //ת��״̬0
				}
				else if((Key_Buf[i].KEY_LEVEL == 1) && (++Key_Buf[i].KEY_COUNT >= KEY_SUPER_LONG_DOWN_DELAY)) 
				{
					Key_Buf[i].KEY_COUNT = 0;//����������
					Key_Buf[i].KEY_EVENT = KEY_SUPER_LONG;//�������¼�
					Key_Buf[i].KEY_STATUS = KEY_SUPER_LONG;//ת��״̬4
				}
				else
				{
					Key_Buf[i].KEY_EVENT = KEY_NULL;//���¼�
				}
				break;
				//״̬4������������
			case KEY_SUPER_LONG:
				if (Key_Buf[i].KEY_LEVEL != 1) //�����ͷţ��˿ڸߵ�ƽ
				{
					Key_Buf[i].KEY_EVENT = KEY_UP;//�ɿ��¼�
					Key_Buf[i].KEY_STATUS = KEY_NULL; //ת��״̬0
				}
				else if((Key_Buf[i].KEY_LEVEL == 1) && (++Key_Buf[i].KEY_COUNT >= KEY_SUPER_LONG_DOWN_DELAY)) 
				{
					Key_Buf[i].KEY_COUNT = 0;//����������
					Key_Buf[i].KEY_EVENT = KEY_SUPER_LONG;//�������¼�
					Key_Buf[i].KEY_STATUS = KEY_SUPER_LONG;//ת��״̬4
				}
				else
				{
					Key_Buf[i].KEY_EVENT = KEY_NULL;//���¼�
				}
				break;
			
			default:
			break;
		}
	}
	
} 

u8 tset(void)
{
	u16 state;
	
//	Get_Key_Level();

//  state = Key_Buf[0].KEY_LEVEL | (Key_Buf[1].KEY_LEVEL << 1) | (Key_Buf[2].KEY_LEVEL << 2);	
	state = Key_Buf[2].KEY_STATUS;
	
	return state;
}











