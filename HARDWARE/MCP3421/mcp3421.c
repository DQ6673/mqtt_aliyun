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
   //写入地址   及 最后位为0，是写操作配置   最后位为1，是读操作配置
  IIC_Send_Byte(0xd0);   // 1101 a2 a1 a0 0  发送给第一个字节数据 MCP3421地址字节+R/W命令
                                 // 1101 0000 0xd0 写模式
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
	IIC_Send_Byte(0xd1);                    //0xd1=0b11010001, 最后一位1表示单片机接收数据
	IIC_Wait_Ack();                     	 	//MCP3421发出应答ACK信号
	
	//读取第二个字节数据 Upper Data Byte
	elech=(long int)IIC_Read_Byte(1);       //NULL NULL NULL NULL NULL NULL D17 D16
	//读取第三个字节数据 Lower Data Byte
	elecm=(long int)IIC_Read_Byte(1);       //D15 D14 D13 D12 D11 D10 D9 D8

	elecl=(long int)IIC_Read_Byte(1);       //D7 D6 D5 D4 D3 D2 D1 D0

	config = (long int)(IIC_Read_Byte(0));  //RDY C1 C0 O/C S1 S0 G1 G0

	IIC_Stop();

	//开始进行数据转化
	AD_B_Result=(elech<<16)|(elecm<<8)|(elecl); //将三个数据进行整合，整合成一个24位的数据


	AD_B_Result=AD_B_Result&0x01ffffff;          //由于数据elech的前6个字节没有用，所以将其清零

	LSB = (float)(2.0 * 2.048) / 262144;  //公式 (2*Vref(2.048))/2^n n为分辨率 12,14,16,18 
									 //18为计算为15.625uv
	PGA = 1; //增益为1
	AD_F_Result = (float)(LSB * AD_B_Result)/PGA; //输出范围 -2.048v-0v-2.048v
	return AD_F_Result;
}





float get_adc(void)
{
	int i = 0;
	float sum = 0.0;
	float data = 0.0;
	//为了稳定获取10次求平均数
	for(i=0; i<10; i++)
	{
		sum += READ_MCP3421();
	}
	//转化为实际电压
	data = 100-((sum / 10)* 20 * 100/40.96);
	
	return data; 
}










