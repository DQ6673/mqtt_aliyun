#ifndef __BUZZER_H
#define __BUZZER_H

#include "sys.h"

#define	buz_port	GPIOB
#define buz_pin		GPIO_Pin_8


void buzzer_init(void);
void buzzer_chirping(u32 , u32 );


#endif




