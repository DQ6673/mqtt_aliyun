#ifndef __RELAY_H
#define __RELAY_H

#include "sys.h"


#define pump_port		GPIOC
#define pump_pin		GPIO_Pin_13
#define fan_port		GPIOA
#define fan_pin			GPIO_Pin_4


void relay_init(void);
void pump_on(void);
void pump_off(void);
void fan_on(void);
void fan_off(void);


#endif


