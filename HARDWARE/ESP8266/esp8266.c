#include "esp8266.h"
#include "usart.h"	  
#include "string.h"
#include "delay.h"

//u8  IN_Buf_flag = 0;
//u8  Buf_len_num = 0;

//u16 USART_RX_STA=0;       		//接收状态标记	  

u16  res_sta = 0;
u16  res_cnt = 0;

char	esprx_buf[rx_len] = {0};			
char 	esptx_buf[tx_len] = {0};
u16 findack_timeout		= 0;
u8  esprx_flag = 1;
//tx buf : A T + N    ____
//				 0 1 2 3    4


//AT+CWJAP="HUAWEI P20","123456789"
void esp_wificonnect(char *apname, char *appassword)
{
	clear_txbuf();
	sprintf(esptx_buf, "AT+CWJAP=\"%s\",\"%s\"\r\n", apname, appassword);
	esp_sendmassage();
	delay_ms(10);
}

//AT+MQTTUSERCFG=0,1,"cid","user","password",0,0,""
void esp_mqttusrcfg(char *cid, char *muser, char*mpsw)
{
	clear_txbuf();
	sprintf(esptx_buf, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", cid, muser, mpsw);
	esp_sendmassage();
	delay_ms(10);
}



void esp_mqttconnect(char *addr, char *port)
{
	clear_txbuf();
	sprintf(esptx_buf, "AT+MQTTCONN=0,\"%s\",%s,1\r\n", addr, port);
	esp_sendmassage();
	delay_ms(10);
}



void esp8266_init(void)
{
	USART2_Init(115200);
	esp_reset();
}





void esp_reset(void)
{
	u8 i=1;
	clear_rxbuf();
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(500);
	esp_sendstring("AT\r\n");
	esp_sendstring("AT+RESTORE\r\n");
//	while(strstr(esprx_buf, "OK") != NULL && i--)
//	{
//		esp_sendstring("AT\r\n");
//		delay_ms(100);
//	}
	//esp_sendstring("AT+RST\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(500);
}


void esp_set_station(void)
{
	clear_rxbuf();
	esp_sendstring("AT+CWMODE=1\r\n");
}




/****************************************************************************************/
void esp_sendstring(char *str)
{
	clear_rxbuf();
	while(*str)
	{
		USART_SendData(USART2,*str++);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
	delay_ms(5);
}



void esp_sendmassage(void)
{
	u16 cnt = 0;
	
	clear_rxbuf();
	while(esptx_buf[cnt++]!='\0' && cnt<=tx_len)	cnt++;			// find the end '\0' or lenth out
	if(cnt>tx_len) cnt=0;										
	
	USART_SendString(USART2, (u8 *)&esptx_buf[0], cnt);
}


void clear_rxbuf(void)
{
	res_cnt = 0;
	memset(esprx_buf, 0, rx_len);
}


void clear_txbuf(void)
{
	memset(esptx_buf, 0, tx_len);
}

/************************************************************************************/




void USART2_IRQHandler(void)                
{
	static u8 Res = 0;
		
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART2,USART_IT_RXNE);
		Res = USART_ReceiveData(USART2);
		
		esprx_buf[res_cnt++] = Res;
	}
}



/****************************************************************************/


void USART_SendString(USART_TypeDef* USARTx,u8 *pbuff,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USARTx,*pbuff++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}




void USART2_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2); 
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	USART_InitStrue.USART_BaudRate=baud;
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; 
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; 
	USART_InitStrue.USART_Parity=USART_Parity_No; 
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStrue);
	
	
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStrue);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_Cmd(USART2,ENABLE);
}











