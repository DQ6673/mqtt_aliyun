#include "syscfg.h"

u32 stm32mcuUID;
char lcd_buf[20] = {0};
PAGE_MODE pagemode = PAGE_IDLE;
ESP_STATE espstate = ESP_IDLE;

float temp_now = 0;
float temp_set = 0;
float hum_now = 0;
float hum_set = 0;

u16	mqtt_pub_cnt_ms = 0;
/********************************************************************/
//WiFi information
char *AP_username 		= "wangshanglaogong";
char *AP_password 		= "12345677";

/********************************************************************/
//mqtt server login infomation
//char *Address		 			= "gxv6n6NiKem.iot-as-mqtt.cn-shanghai.aliyuncs.com";
//char *Port			   		= "1883";
//char *ClintID 				= "gxv6n6NiKem.smartdeviceboard|securemode=2\\,signmethod=hmacsha256\\,timestamp=1647760460431|";                                                                                               
//char *MQTT_username  	= "smartdeviceboard&gxv6n6NiKem";
//char *MQTT_password  	= "74807f5c9885c282128105b4e3a41fccbb85cb6c9c98e501c1ec5e2c9457168a";

//char *MQTT_pub_topic 	= "/sys/gxv6n6NiKem/smartdeviceboard/thing/event/property/post";

/********************************************************************/

char *Address		 			= "gzo1XbeINXN.iot-as-mqtt.cn-shanghai.aliyuncs.com";
char *Port			   		= "1883";
char *ClintID 				= "gzo1XbeINXN.smartboard|securemode=2\\,signmethod=hmacsha256\\,timestamp=1648120638229|";                                                                                               
char *MQTT_username  	= "smartboard&gzo1XbeINXN";
char *MQTT_password  	= "7ffdfe08900821b74eabd1aea99a6e2b8ee6f46ab3d7bc9fd201c36226e960ce";

char *MQTT_pub_topic 	= "/sys/gzo1XbeINXN/smartboard/thing/event/property/post";

/********************************************************************/

u8 cou = 0;
u16 ii=0;
/********************************************************************/
void systemconfig(void)
{
	GetUID();													//获取设备ID
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//配置中断分组
	
	delay_init();								//延时函数初始化
	
	tim4_init();								//定时器初始化
	
	lcd1602_init();							//LCD初始化
	key_init();									//按键初始化
	buzzer_init();							//蜂鸣器初始化
	relay_init();								//继电器初始化
	ds18b20_init();							//温度传感器初始化
	
	IIC_Init();									//IIC初始化
	AT24CXX_Init();							//EEPROM初始化
	AT24CXX_WriteOneByte(0X0060,0X16);		//检验EEPROM功能
	cou = AT24CXX_ReadOneByte(0X0060);		//
		
	mcp3421_init();							//ADC初始化
	
	if(!esp_debug) 
		esp8266_init();						//ESP8266初始化
	
	temp_set = AT24CXX_ReadOneByte(tempset_addr);		//读取温度设定值
	hum_set = AT24CXX_ReadOneByte(humset_addr);			//读取湿度设定值
	pagemode = DATADISPLAY;													//LCD页面
}



/********************************************************************/
void get_data(void)
{
	temp_now = get_temperature();		//获取温度
	hum_now = get_adc();						//获取湿度
	//espstate_check();
}





/********************************************************************/
void massage_handler(void)
{
	keyvalue_handler();
	
	if(!esp_debug) 
		network_handler();
	
	relay_handler();
}




/********************************************************************/
void display_reflash(void)
{
	static PAGE_MODE page_now = PAGE_IDLE;
	switch(pagemode)
	{
		case DATADISPLAY : 	
			
		if(pagemode != page_now) 
		{
			LCD_Clear();
			page_now = DATADISPLAY;
		}
		LCD_Show_Str(0, 0, "Temp:");
		LCD_Show_Str(8, 0, "Hum:");
		sprintf(lcd_buf, "%.2fC ",temp_now);
		LCD_Show_Str(0, 1, lcd_buf);
		sprintf(lcd_buf, "%.2f%% ",hum_now);
		LCD_Show_Str(8, 1, lcd_buf);
		
		if(espstate == MQTTPUB)						LCD_Show_Str(15, 0, "^");
		else if(espstate == MQTTUSRCFG)		LCD_Show_Str(15, 0, "|");
		else 															LCD_Show_Str(15, 0, "!");
		break;
		
		
		case SETTING_TEMP :
			
		if(pagemode != page_now) 
		{
			LCD_Clear();
			page_now = SETTING_TEMP;
		}
		LCD_Show_Str(0, 0, "Setting Temp:");
		sprintf(lcd_buf, "%.2fC ",temp_set);
		LCD_Show_Str(0, 1, lcd_buf);
		break;
		
		
		case SETTING_HUM :
			
		if(pagemode != page_now) 
		{
			LCD_Clear();
			page_now = SETTING_HUM;
		}
		LCD_Show_Str(0, 0, "Setting Hum:");
		sprintf(lcd_buf, "%.2f%% ",hum_set);
		LCD_Show_Str(0, 1, lcd_buf);
		break;
		
		
		case CONNECT_INFO :
			
		if(pagemode != page_now) 
		{
			LCD_Clear();
			page_now = CONNECT_INFO;
		}
		switch(espstate)
		{
			case MQTTUSRCFG :		LCD_Show_Str(0, 0, "WIFI connected");
											break;
			
			case MQTTPUB :	LCD_Show_Str(0, 0, "MQTT connected");
											break;
				
			default : LCD_Show_Str(0, 0, "No connection!");
								LCD_Show_Str(0, 1, "---.---.---.---");
								break;
		}
		break;
		
		
		default : break;
	}
}





/********************************************************************/
void keyvalue_handler(void)
{
	if(key_mode_flag || key_back_flag || key_up_flag || key_down_flag)
	{
		buzzer_chirping(4000, 50);
		switch(pagemode)
		{
			case DATADISPLAY : 
				
			if(key_mode_flag)	{pagemode = SETTING_TEMP; key_mode_flag=0;}
			if(key_back_flag)	key_back_flag=0;
			if(key_up_flag)		key_up_flag=0;
			if(key_down_flag)	key_down_flag=0;
			break;
			
			
			case SETTING_TEMP :	

			if(key_mode_flag)	
			{
				AT24CXX_WriteOneByte(tempset_addr, temp_set);
				pagemode = SETTING_HUM; 
				key_mode_flag=0;
			}
			if(key_back_flag)	
			{
				AT24CXX_WriteOneByte(tempset_addr, temp_set);
				pagemode = DATADISPLAY; 
				key_back_flag=0;
			}
			if(key_up_flag)		
			{
				if(temp_set < 100) temp_set += 1;
				key_up_flag=0;
			}
			if(key_down_flag)	
			{
				if(temp_set > 0) temp_set -= 1;
				key_down_flag=0;
			}
			
			
			case SETTING_HUM :

			if(key_mode_flag)	
			{
				AT24CXX_WriteOneByte(humset_addr, hum_set);
				pagemode = CONNECT_INFO; 
				key_mode_flag=0;
			}
			if(key_back_flag)	
			{
				AT24CXX_WriteOneByte(humset_addr, hum_set);
				pagemode = DATADISPLAY; 
				key_back_flag=0;
			}
			if(key_up_flag)		
			{
				if(hum_set < 100) hum_set += 1;
				key_up_flag=0;
			}
			if(key_down_flag)	
			{
				if(hum_set > 0) hum_set -= 1;
				key_down_flag=0;
			}			
			
			
			case CONNECT_INFO :
				
			if(key_mode_flag)	{pagemode = DATADISPLAY; key_mode_flag=0;}
			if(key_back_flag)	{pagemode = DATADISPLAY; key_back_flag=0;}
			if(key_up_flag)		key_up_flag=0;
			if(key_down_flag)	key_down_flag=0;
			break;
			
			default : break;
		}
	}
}



/********************************************************************/
void network_handler(void)
{
	static ESP_STATE sendflag = ESP_IDLE;
	
	switch(espstate)
	{
		case ESP_IDLE:
			
		sendflag = SET_STATION;
		espstate = SET_STATION;
		break;
		
		
		case SET_STATION:
			
		if(sendflag == SET_STATION)	
		{
			ii=9;
			esp_set_station();
			delay_ms(20);
			sendflag = WIFICONN;
		}
		if(strstr(esprx_buf, "OK") != NULL) espstate = WIFICONN;
		break;
		
		
		case WIFICONN:
			
		if(sendflag == WIFICONN)	
		{
			esp_wificonnect(AP_username, AP_password);
			sendflag = MQTTUSRCFG;
		}
		if(strstr(esprx_buf, "WIFI GOT IP") != NULL) espstate = MQTTUSRCFG;
		break;
		
		
		case MQTTUSRCFG:
			
		if(sendflag == MQTTUSRCFG)	
		{
			esp_mqttusrcfg(ClintID, MQTT_username, MQTT_password);
			sendflag = MQTTCONN;
		}
		if(strstr(esprx_buf, "OK") != NULL) espstate = MQTTCONN;
		break;
		
		
		case MQTTCONN:
			
		if(sendflag == MQTTCONN)	
		{
			esp_mqttconnect(Address, Port);
			sendflag = MQTTPUB;
		}
		if(strstr(esprx_buf, "1883") != NULL) espstate = MQTTPUB;
		break;
		
		
		case MQTTPUB:
			
		if(mqtt_pub_cnt_ms >= mqtt_pub_cnt_set)
		{
			mqtt_publish();
			mqtt_pub_cnt_ms = 0;
		}
		break;
		
		
		default : break;
	}
}



/********************************************************************/
void mqtt_publish(void)
{
	clear_txbuf();
	
	sprintf(esptx_buf, 
	"AT+MQTTPUB=0,"
	"\"%s\","
	"\"{"
	"\\\"id\\\": \\\"6666666666666\\\"\\,"
	"\\\"params\\\":{\\\"ATemp\\\":%.2f\\,\\\"sHum\\\":%.2f}\\,"
	"\\\"version\\\":\\\"1.0\\\"\\,"
	"\\\"method\\\":\\\"thing.event.property.post\\\"}\","
	"1,0"
	"\r\n"
	,MQTT_pub_topic
	,temp_now
	,hum_now
	);
	
	esp_sendmassage();
	delay_ms(10);
}



/********************************************************************/
void relay_handler(void)
{
	if(temp_now >= temp_set)	fan_on();
	else											fan_off();
	
	if(hum_now <= hum_set)		pump_on();
	else											pump_off();
}



//获取stm32f1xxxx芯片的唯一ID函数
void GetUID(void)
{    
  stm32mcuUID += *(__IO u32*)(0X1FFFF7F2);//ID地址的高32位寄存器地址
  stm32mcuUID += *(__IO u32*)(0X1FFFF7EE);//ID地址的中32位寄存器地址
  stm32mcuUID += *(__IO u32*)(0X1FFFF7E8);//ID地址的低32位寄存器地址
	//stm32mcuUID += 2;
}


