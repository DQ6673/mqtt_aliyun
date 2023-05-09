#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"


//IO方向设置
//#define SDA_IN()  {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=0x08000000;}
//#define SDA_OUT() {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=0x03000000;}

#define SDA_IN()  {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=0x00000008;}
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=0x00000003;}


//IO操作函数	 
//#define IIC_SCL    PAout(7) //SCL
//#define IIC_SDA    PAout(6) //SDA	 
//#define READ_SDA   PAin(6)  //输入SDA 

#define IIC_SCL    PAout(7) //SCL
#define IIC_SDA    PBout(0) //SDA	 
#define READ_SDA   PBin(0)  //输入SDA 
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















