#include "syscfg.h"

/**********************************************/
//project name : wifi-sensor
//Finished date : 2022/3/23
//config setting : syscfg.c
/**********************************************/


int main(void){

	systemconfig();		//��ʼ��
	
	while(1)
	{
		get_data();	//��ȡ�¶�������ʪ��
		massage_handler();	//��Ϣ�����뷢��
		display_reflash();	//ˢ����ʾ
	}
}










