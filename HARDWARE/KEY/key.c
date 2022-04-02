#include "key.h"
#define uint8_t unsigned char
#define KEY_LONG_DOWN_DELAY 30 
#define KEY_SUPER_LONG_DOWN_DELAY 100
/*①按键屏蔽 0:屏蔽 1:不屏蔽、②按键长按计数、③虚拟当前IO电平,按下1,抬起0
④按下时IO实际电平、⑤按键状态、⑥按键事件、⑦读IO电平函数
*/

KEY_COMPONENTS Key_Buf[KEY_NUM] = 
{
	{1,0,0,0,KEY_NULL,KEY_NULL,KEY0_ReadPin},
	{1,0,0,0,KEY_NULL,KEY_NULL,KEY1_ReadPin},
	{1,0,0,1,KEY_NULL,KEY_NULL,KEY2_ReadPin},
	
};

/*像WK_UP就是共阳极接法，引脚需要下拉，按钮按下的时候是高电平，松开是低电平，
而KEY0和KEY1就是共阴极解法，引脚需要上拉，按钮按下的时候是低电平，而松开的时候是高电平。*/
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

void KEY_Init(void)//按键初始化
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
	
} 


// -------------------------------按键状态机
void ReadKeyStatus(void)
{
	uint8_t i;
	Get_Key_Level();
	for ( i = 0; i < KEY_NUM; i++)
	{
		switch (Key_Buf[i].KEY_STATUS)//有按键按下
		{	//状态0：按键没有按下
			case KEY_NULL:
				if (Key_Buf[i].KEY_LEVEL == 1) //触发条件：有按键按下
				{
					Key_Buf[i].KEY_STATUS = KEY_SURE; //转入状态1
					Key_Buf[i].KEY_EVENT = KEY_NULL; //空事件
				}
				else
				{
					Key_Buf[i].KEY_STATUS = KEY_NULL;//未发生事件，继续保持原状态
				}
				break;
			//状态1：按键按下确认	
			case KEY_SURE:
				if (Key_Buf[i].KEY_LEVEL == 1)//确认按键依然处于按下状态
				{
					Key_Buf[i].KEY_EVENT = KEY_DOWN;//按下事件
					Key_Buf[i].KEY_STATUS = KEY_DOWN; //转入状态2
					Key_Buf[i].KEY_COUNT = 0; //计数器清零
				}
				else
				{
					Key_Buf[i].KEY_STATUS = KEY_NULL;//转入状态0
					Key_Buf[i].KEY_EVENT = KEY_NULL;//空事件
				}
				break;
				//状态2：按键按下
			case KEY_DOWN:
				if (Key_Buf[i].KEY_LEVEL != 1) //按键释放，端口高电平
				{
					Key_Buf[i].KEY_EVENT = KEY_UP; //松开事件
					Key_Buf[i].KEY_STATUS = KEY_NULL; //转入状态0
				}
				else if ((Key_Buf[i].KEY_LEVEL == 1) && (++Key_Buf[i].KEY_COUNT >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
				{
					Key_Buf[i].KEY_EVENT = KEY_LONG;//长按事件
					Key_Buf[i].KEY_STATUS = KEY_LONG; //转为状态3
					Key_Buf[i].KEY_COUNT = 0;//计数器清零
				}
				else
				{
					Key_Buf[i].KEY_EVENT = KEY_NULL;//空事件
				}
				break;
				//状态3：按键长按
			case KEY_LONG:
				if (Key_Buf[i].KEY_LEVEL != 1) //按键释放，端口高电平
				{
					Key_Buf[i].KEY_EVENT = KEY_UP;//松开事件
					Key_Buf[i].KEY_STATUS = KEY_NULL; //转入状态0
				}
				else if((Key_Buf[i].KEY_LEVEL == 1) && (++Key_Buf[i].KEY_COUNT >= KEY_SUPER_LONG_DOWN_DELAY)) 
				{
					Key_Buf[i].KEY_COUNT = 0;//计数器清零
					Key_Buf[i].KEY_EVENT = KEY_SUPER_LONG;//超长按事件
					Key_Buf[i].KEY_STATUS = KEY_SUPER_LONG;//转入状态4
				}
				else
				{
					Key_Buf[i].KEY_EVENT = KEY_NULL;//空事件
				}
				break;
				//状态4：按键超长按
			case KEY_SUPER_LONG:
				if (Key_Buf[i].KEY_LEVEL != 1) //按键释放，端口高电平
				{
					Key_Buf[i].KEY_EVENT = KEY_UP;//松开事件
					Key_Buf[i].KEY_STATUS = KEY_NULL; //转入状态0
				}
				else if((Key_Buf[i].KEY_LEVEL == 1) && (++Key_Buf[i].KEY_COUNT >= KEY_SUPER_LONG_DOWN_DELAY)) 
				{
					Key_Buf[i].KEY_COUNT = 0;//计数器清零
					Key_Buf[i].KEY_EVENT = KEY_SUPER_LONG;//超长按事件
					Key_Buf[i].KEY_STATUS = KEY_SUPER_LONG;//转入状态4
				}
				else
				{
					Key_Buf[i].KEY_EVENT = KEY_NULL;//空事件
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











