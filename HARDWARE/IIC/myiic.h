#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"


//IO��������
//#define SDA_IN()  {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=0x08000000;}
//#define SDA_OUT() {GPIOA->CRL&=0XF0FFFFFF;GPIOA->CRL|=0x03000000;}

#define SDA_IN()  {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=0x00000008;}
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFFF0;GPIOB->CRL|=0x00000003;}


//IO��������	 
//#define IIC_SCL    PAout(7) //SCL
//#define IIC_SDA    PAout(6) //SDA	 
//#define READ_SDA   PAin(6)  //����SDA 

#define IIC_SCL    PAout(7) //SCL
#define IIC_SDA    PBout(0) //SDA	 
#define READ_SDA   PBin(0)  //����SDA 
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















