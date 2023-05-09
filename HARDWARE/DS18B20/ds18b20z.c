#include "ds18b20z.h"
#include "delay.h"


u16 temptrans_cnt_ms = 0;
u8 temptrans_sta = 0;

void mode_output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure Ports */
	GPIO_InitStructure.GPIO_Pin = DQ_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DQ_port, &GPIO_InitStructure);
}


void mode_input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	/* Configure Ports */
	GPIO_InitStructure.GPIO_Pin = DQ_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DQ_port, &GPIO_InitStructure);
}


void ds18b20_rst(void)
{
  mode_output();               
  GPIO_ResetBits(DQ_port,DQ_pin);             
  delay_us(600);        
  GPIO_SetBits(DQ_port,DQ_pin);            
  mode_input();
  while(GPIO_ReadInputDataBit(DQ_port, DQ_pin));         
  while(!GPIO_ReadInputDataBit(DQ_port, DQ_pin));        
}


void ds18b20_write_zero(void)
{
  mode_output();                   
  GPIO_ResetBits(DQ_port,DQ_pin);                  
  delay_us(80);            
  GPIO_SetBits(DQ_port,DQ_pin);                 
  delay_us(2);
} 


void ds18b20_write_one(void)
{
  mode_output();                          
  GPIO_ResetBits(DQ_port,DQ_pin);
  delay_us(2);
  GPIO_SetBits(DQ_port,DQ_pin);
  delay_us(80);
}


unsigned char ds18b20_read_bit(void)
{
  unsigned char data;
  mode_output();
  GPIO_ResetBits(DQ_port,DQ_pin);
  delay_us(2);
  GPIO_SetBits(DQ_port,DQ_pin);;
  mode_input();
  delay_us(5);
  if(GPIO_ReadInputDataBit(DQ_port, DQ_pin))	data=1;
  else	data=0;
  delay_us(500);  
  return data;
}
 

void ds18b20_write_byte(unsigned char data)
{
  unsigned char i,testb;
  mode_output();
  for(i=0;i<8;i++)
  {
    testb=data&0x01;                
    data>>=1;
    if(testb)
      ds18b20_write_one();
    else
      ds18b20_write_zero();
  }
}
 

unsigned char ds18b20_read_byte(void)
{
  unsigned char i,j,data=0;
  for(i=0;i<8;i++)
  {
    j=ds18b20_read_bit();
    data=(j<<7)|(data>>1);  
  }
  return data;
}
 

void ds18b20_start(void)
{                                              
   ds18b20_rst();           
   ds18b20_write_byte(0xcc);
   ds18b20_write_byte(0x44);
}


void ds18b20_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  ds18b20_rst();
} 


//return:0 or temp
float get_temperature(void)
{
	unsigned char TL=0,TH=0;
	short tem=0;
	static float temperature = 0;
		
	switch(temptrans_sta)
	{
		case 0 : 	ds18b20_start();
							temptrans_cnt_ms = 0;
							temptrans_sta = 1;
							break;
		
		case 1 :	if(temptrans_cnt_ms > 1000)
							{
								temptrans_sta = 2;
							}
							break;
		
		case 2 :	ds18b20_init();    
							ds18b20_write_byte(0xcc);   		
							ds18b20_write_byte(0xbe);   		
							TL=ds18b20_read_byte();     		
							TH=ds18b20_read_byte();     		    
							tem=TH;                      		
							tem<<=8;   
							tem+=TL;                     		
							if((TH>>7)&0x01)	temperature = (~tem + 1) * 0.0625;
							else							temperature = tem * 0.0625;
							temptrans_sta = 0;
							break;
		
		default : temptrans_sta = 0;
							break;
	}
	
	return temperature;    
}














