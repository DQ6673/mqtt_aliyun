#include "relay.h"

/*************************************************************/
//functions for controlling relays : pump & fan
/*************************************************************/

//		pump 	: PA5
//		fan		: PA4



void relay_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 		//enable GPIO port A
	
	GPIO_InitStructure.GPIO_Pin = fan_pin | pump_pin;	 	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	pump_off();
	fan_off();
}


void pump_off(void)
{
	GPIO_SetBits(pump_port, pump_pin);
}


void pump_on(void)
{
	GPIO_ResetBits(pump_port, pump_pin);
}


void fan_off(void)
{
	GPIO_SetBits(fan_port, fan_pin);
}


void fan_on(void)
{
	GPIO_ResetBits(fan_port, fan_pin);
}




