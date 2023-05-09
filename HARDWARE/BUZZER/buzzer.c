#include "buzzer.h"
#include "delay.h"

/*************************************************************/
//functions for controlling buzzer
/*************************************************************/

//		buzzer output pin	: PB8
u32 freq_num=0;
u32 time_num=0;


/*************************************************************/
void buzzer_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//enable GPIO port B
	
	GPIO_InitStructure.GPIO_Pin = buz_pin;	 	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 					//push/pull  mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/*************************************************************/
//freq : Hz
//time : ms
void buzzer_chirping(u32 freq, u32 time)
{
	freq_num = 1000000 / (freq*2);
	time_num = time * 1000 / (2*freq_num);
	
	for(u32 i=0; i<=time_num; i++)								//for 20ms
	{
		GPIO_SetBits(buz_port, buz_pin);
		delay_us(freq_num);												//4000Hz
		GPIO_ResetBits(buz_port, buz_pin);
		delay_us(freq_num);
	}
}




