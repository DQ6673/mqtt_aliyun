#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"


#define rx_len 500
#define tx_len 300

extern char	esprx_buf[rx_len];			
extern char esptx_buf[tx_len];


void esp8266_init(void);
void esp_sendmassage(void);
void clear_rxbuf(void);
void clear_txbuf(void);
void esp_sendstring(char *str);
void esp_sendmassage(void);
void esp_reset(void);
void esp_set_station(void);
void esp_wificonnect(char *apname, char *appassword);
void esp_mqttusrcfg(char *cid, char *muser, char*mpsw);
void esp_mqttconnect(char *addr, char *port);
	
void USART2_Init(u32 baud);
void USART_SendString(USART_TypeDef* USARTx,u8 *pbuff,u8 len);


#endif





