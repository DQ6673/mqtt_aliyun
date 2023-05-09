#include "lcd1602.h"
#include "delay.h"

//functions for LCD1602 display
/**********************************************/


void lcd1602_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 		//enable GPIO port A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 		//enable GPIO port B
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 			//enable AF clock
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //disable JTAG port
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|
																GPIO_Pin_9|
																GPIO_Pin_10|
																GPIO_Pin_11|
																GPIO_Pin_12|
																GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|
																GPIO_Pin_12|
																GPIO_Pin_13|
																GPIO_Pin_14|
																GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RS = 0;
	RW = 0;
	E = 0;
	
	LCD_Write_Com(0x38);delay_ms(5);
	LCD_Write_Com(0x01);delay_ms(5);
	LCD_Write_Com(0x0c);delay_ms(5);
	LCD_Write_Com(0x06);delay_ms(5);
	
	LCD_Show_Str(0, 0, "Loading...");
}



void LCD_Write_Com(u8 com)
{
	RS = 0;
	RW = 0;
	DB7 = (com >> 7) & 0x01;
	DB6 = (com >> 6) & 0x01;
	DB5 = (com >> 5) & 0x01;
	DB4 = (com >> 4) & 0x01;
	DB3 = (com >> 3) & 0x01;
	DB2 = (com >> 2) & 0x01;
	DB1 = (com >> 1) & 0x01;
	DB0 = (com >> 0) & 0x01;
	E = 1;
	delay_ms(5);
	E = 0;
}



void LCD_Write_Data(u8 dat)
{
	RS = 1;
	RW = 0;
	DB7 = (dat >> 7) & 0x01;
	DB6 = (dat >> 6) & 0x01;
	DB5 = (dat >> 5) & 0x01;
	DB4 = (dat >> 4) & 0x01;
	DB3 = (dat >> 3) & 0x01;
	DB2 = (dat >> 2) & 0x01;
	DB1 = (dat >> 1) & 0x01;
	DB0 = (dat >> 0) & 0x01;
	E = 1;
	delay_ms(5);
	E = 0;
}



void LCD_Set_Cursor(u8 x, u8 y)
{
	u8 addr;
	 
	if (y == 0)
	addr = 0x00 + x;
	else
	addr = 0x40 + x;
	LCD_Write_Com(addr | 0x80);
}



void LCD_Show_Str(u8 x, u8 y, char *str)
{
	LCD_Set_Cursor(x, y);
	while(*str != '\0')
	{
		LCD_Write_Data(*str++);
	}
}


void LCD_Clear(void)
{
	LCD_Write_Com(0x01);
	delay_ms(5);
}




