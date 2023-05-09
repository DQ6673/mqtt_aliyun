#include "mcp3421.h"
#include "myiic.h"


/**********************************************/
//functions for mcp3421 ADC



void mcp3421_init(void)
{
	Write_MCP3421(0X9C);
}




void Write_MCP3421(unsigned char WR_Data)
{
  IIC_Start();
   //д���ַ   �� ���λΪ0����д��������   ���λΪ1���Ƕ���������
  IIC_Send_Byte(0xd0);   // 1101 a2 a1 a0 0  ���͸���һ���ֽ����� MCP3421��ַ�ֽ�+R/W����
                                 // 1101 0000 0xd0 дģʽ
  IIC_Wait_Ack(); 
  IIC_Send_Byte(WR_Data); //RDY O/C C1 C0 S1 S0 G1 G0    
                         
  IIC_Wait_Ack();
  IIC_Stop();  
}




float READ_MCP3421()
{
	unsigned long int elech;
	unsigned long int elecm;
	unsigned long int elecl;
	unsigned long int config;
	unsigned long int AD_B_Result;
	float AD_F_Result=0.0;

	float LSB;
	int PGA;
	IIC_Start();
	IIC_Send_Byte(0xd1);                    //0xd1=0b11010001, ���һλ1��ʾ��Ƭ����������
	IIC_Wait_Ack();                     	 	//MCP3421����Ӧ��ACK�ź�
	
	//��ȡ�ڶ����ֽ����� Upper Data Byte
	elech=(long int)IIC_Read_Byte(1);       //NULL NULL NULL NULL NULL NULL D17 D16
	//��ȡ�������ֽ����� Lower Data Byte
	elecm=(long int)IIC_Read_Byte(1);       //D15 D14 D13 D12 D11 D10 D9 D8

	elecl=(long int)IIC_Read_Byte(1);       //D7 D6 D5 D4 D3 D2 D1 D0

	config = (long int)(IIC_Read_Byte(0));  //RDY C1 C0 O/C S1 S0 G1 G0

	IIC_Stop();

	//��ʼ��������ת��
	AD_B_Result=(elech<<16)|(elecm<<8)|(elecl); //���������ݽ������ϣ����ϳ�һ��24λ������


	AD_B_Result=AD_B_Result&0x01ffffff;          //��������elech��ǰ6���ֽ�û���ã����Խ�������

	LSB = (float)(2.0 * 2.048) / 262144;  //��ʽ (2*Vref(2.048))/2^n nΪ�ֱ��� 12,14,16,18 
									 //18Ϊ����Ϊ15.625uv
	PGA = 1; //����Ϊ1
	AD_F_Result = (float)(LSB * AD_B_Result)/PGA; //�����Χ -2.048v-0v-2.048v
	return AD_F_Result;
}





float get_adc(void)
{
	int i = 0;
	float sum = 0.0;
	float data = 0.0;
	//Ϊ���ȶ���ȡ10����ƽ����
	for(i=0; i<10; i++)
	{
		sum += READ_MCP3421();
	}
	//ת��Ϊʵ�ʵ�ѹ
	data = 100-((sum / 10)* 20 * 100/40.96);
	
	return data; 
}










