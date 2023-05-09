#include "syscfg.h"

/**********************************************/
//project name : wifi-sensor
//Finished date : 2022/3/23
//config setting : syscfg.c
/**********************************************/


int main(void){

	systemconfig();		//初始化
	
	while(1)
	{
		get_data();	//获取温度与土壤湿度
		massage_handler();	//信息处理与发送
		display_reflash();	//刷新显示
	}
}










