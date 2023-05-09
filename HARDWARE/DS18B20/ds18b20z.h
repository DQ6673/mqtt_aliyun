#ifndef __DS18B20Z_H
#define __DS18B20Z_H

#include "sys.h"


#define		DQ_port		GPIOB
#define		DQ_pin		GPIO_Pin_9

void mode_output(void);
void mode_input(void);
void ds18b20_rst(void);
void ds18b20_write_zero(void);
void ds18b20_write_one(void);
unsigned char ds18b20_read_bit(void);
void ds18b20_write_byte(unsigned char data);
unsigned char ds18b20_read_byte(void);
void ds18b20_start(void);
void ds18b20_init(void);
float get_temperature(void);

#endif








