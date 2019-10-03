/*******************************************************
2016�й���������������˫����̨������С������
оƬ��STC12C5A60S2
����11.0592
��Χ������	0.4W���ⷢ���			6
			3W���ʺ��ⷢ���		1
			8050������				12
			��ѹģ��				1
********************************************************/
#include "INSend.h"
#include "ADC.h"
#include "delay.h"
#include "uart.h"
void main(void) 
{  
	unsigned char Motor;
	unsigned int Motor_temp;
	INSend_Init();
	InitADC();
	Uart_Init();
	while(1) 
	{ 
		unsigned char i;
		Motor_temp=0;
		for(i=0;i<100;i++)
		{
			Motor_temp+=GetADCResult(0); 
		}
		Motor_temp/=1600;									//ȡƽ��ֵ
		Motor_temp=15-Motor_temp;					//X��ȡ��
		Motor=(unsigned char)Motor_temp;
		Motor<<=4;												//װ�ظ���λ����
		Motor_temp=0;
		for(i=0;i<100;i++)
		{
			Motor_temp+=(GetADCResult(1)); 
		}
		Motor_temp/=1600;									//Y��
		Motor|=(unsigned char)Motor_temp; //װ�ص���λ����
		
		SendIRdata(Motor);
		printf("%x\n",Motor);
		delay_ms(110);
	} 
}



