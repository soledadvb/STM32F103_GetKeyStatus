#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "key.h"

/*�����ǰ����״̬
**������״̬�ı��������״̬��
**
*/
void Output_Key_Status() 
{	int i;
	static uint8_t  new_status[KEY_NUM],old_status[KEY_NUM];
		for(i = 0;i <KEY_NUM; i++)
			{	old_status[i] = new_status[i];
				new_status[i]= Key_Buf[i].KEY_STATUS;
				if(new_status[i] != old_status[i])
					 printf("key%d %d\n\r",i,Key_Buf[i].KEY_STATUS);				
			}

}

 int main(void)
 {	
	//delay_init();	    	 //��ʱ������ʼ��
	SysTick_Config(72000000/1000);
	uart_init(9600);
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	KEY_Init();

	TIM3_Int_Init(72,19999);//TIM3�ж� 20ms
 
	 while(1)
	{
		
		
		Output_Key_Status();
		
		
//		if (Sys_Time == 0)
//		{			
//		uint8_t i, status;		 
//		ReadKeyStatus();//����״̬��
//			
//		for(i = 0;i <KEY_NUM; i++)
//			{	status = Key_Buf[i].KEY_STATUS;
//				if(status == KEY_DOWN) printf("%d�̰�\n\r",i);
//				if(status == KEY_LONG) printf("%d����\n\r",i);
//			}
//			Sys_Time = 500;
//		}
	}
}

