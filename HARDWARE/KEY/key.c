#include "key.h"
#include "stm32f10x_exti.h"


//		key1	: PB4		MODE
//		key2	: PB5		UP
//		key3	: PB6		DOWN
//		key4	: PB7		BACK

u8 key_mode_flag = 0;
u8 key_up_flag = 0;
u8 key_down_flag = 0;
u8 key_back_flag = 0;

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NvicStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|
																GPIO_Pin_5|
																GPIO_Pin_6|
																GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	
	
	EXTI_InitStructure.EXTI_Line = 	EXTI_Line4|
																	EXTI_Line5|
																	EXTI_Line6|
																	EXTI_Line7;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	

	NvicStruct.NVIC_IRQChannel    = EXTI4_IRQn;
	NvicStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NvicStruct.NVIC_IRQChannelSubPriority = 1;
	NvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NvicStruct);	
	
	NvicStruct.NVIC_IRQChannel    = EXTI9_5_IRQn;
	NvicStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NvicStruct.NVIC_IRQChannelSubPriority = 2;
	NvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NvicStruct);	
}




void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0){
			key_mode_flag = 1;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}



void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
	
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0){
			key_back_flag = 1;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	else if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
	
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0){
			key_up_flag = 1;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	else if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
	
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0){
			key_down_flag = 1;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}









