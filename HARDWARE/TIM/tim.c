#include "tim.h"
#include "sys.h"
#include "ds18b20z.h"

int tim4_test;
unsigned int tim4_count;

void tim4_init(void)      		                             								  									
{
 	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;                 
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);           		
 
	TIM_TimeBaseStructure.TIM_Period = 101;                        //1ms÷–∂œ“ª¥Œ                 
	TIM_TimeBaseStructure.TIM_Prescaler =719; 											
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;           
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );                        

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 					
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 
	NVIC_Init(&NVIC_InitStructure);                               
	TIM_Cmd(TIM4, ENABLE);
}




void TIM4_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{   
		temptrans_cnt_ms ++;
		mqtt_pub_cnt_ms ++;
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update );
	}
}
