#ifndef __LCD1602_H
#define __LCD1602_H

#include "sys.h"


/*****************************************/


#define 	RW		PAout(15)
#define 	RS		PBout(3)
#define 	E			PAout(12)

#define 	DB0		PAout(11)
#define 	DB1		PAout(10)
#define 	DB2		PAout(9)
#define 	DB3		PAout(8)
#define 	DB4		PBout(15)
#define 	DB5		PBout(14)
#define 	DB6		PBout(13)
#define 	DB7		PBout(12)




void lcd1602_init(void);
void LCD_Write_Com(u8 com);
void LCD_Write_Data(u8 dat);
void LCD_Set_Cursor(u8 x, u8 y);
void LCD_Show_Str(u8 x, u8 y, char *str);
void LCD_Clear(void);

#endif




