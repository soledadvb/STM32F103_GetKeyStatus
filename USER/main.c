#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "key.h"

/*输出当前按键状态
**若按键状态改变则输出新状态。
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
	//delay_init();	    	 //延时函数初始化
	SysTick_Config(72000000/1000);
	uart_init(9600);
	LED_Init();		  	//初始化与LED连接的硬件接口
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	KEY_Init();

	TIM3_Int_Init(72,19999);//TIM3中断 20ms
 
	 while(1)
	{
		
		
		Output_Key_Status();
		
		
//		if (Sys_Time == 0)
//		{			
//		uint8_t i, status;		 
//		ReadKeyStatus();//调用状态机
//			
//		for(i = 0;i <KEY_NUM; i++)
//			{	status = Key_Buf[i].KEY_STATUS;
//				if(status == KEY_DOWN) printf("%d短按\n\r",i);
//				if(status == KEY_LONG) printf("%d长按\n\r",i);
//			}
//			Sys_Time = 500;
//		}
	}
}

