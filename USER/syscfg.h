#ifndef __SYSCFG_H
#define __SYSCFG_H


//参数宏
/************************************************************************/

/************************************************************************/



//头文件
/************************************************************************/
//lib include
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"

//system include
#include "usart.h"
#include "delay.h"
#include "sys.h"

//hardware include
#include "tim.h"
#include "relay.h"
#include "buzzer.h"
#include "lcd1602.h"
#include "key.h"
#include "ds18b20z.h"
#include "myiic.h"
#include "24cxx.h"
#include "mcp3421.h"
#include "esp8266.h"


#define	esp_debug	0
#define mqtt_pub_cnt_set 5000

/************************************/
//EEPROM: address		data
//				0x70			temp_set
//				0x71			hum_set

#define  tempset_addr  0x10
#define  humset_addr   0x11
/************************************/



typedef enum
{
	PAGE_IDLE = 0,
	DATADISPLAY,
	SETTING_TEMP,
	SETTING_HUM,
	CONNECT_INFO
	
}PAGE_MODE;

typedef enum
{
	ESP_IDLE = 0,
	SET_STATION,
	WIFICONN,
	GOT_IP,
	MQTTUSRCFG,
	MQTTCONN,
	MQTTPUB
	
}ESP_STATE;


extern u32 stm32mcuUID;

//函数
/************************************************************************/

void systemconfig(void);
void get_data(void);
void massage_handler(void);
void keyvalue_handler(void);
void network_handler(void);
void relay_handler(void);
void display_reflash(void);
void GetUID(void);

void mqtt_publish(void);



#endif
