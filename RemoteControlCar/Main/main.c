/*******************************************************
2018�й���������������˫����̨����ң��С������
оƬ��AT89S52
����11.0592
��Χ������������չ� 	6	
		  ����ѭ�� 		3
		  ����ѹģ��	1
		  6��������		1
********************************************************/
#include "uart.h"
#include "motor.h"
#include "INRecv.h"
void main()
{
	InitTimer1();
	INT0_Init();
	Uart_Init();
	Control = 0xee;
	//printf("while begin~~\r\n");
	while(1)
	{
//		while(!INRecv_flag)			//û���յ�ң�������ź�֪ͨ��ȴ�
//		{
//			if(Lost_counter>2000)	//��ʱ��û�н��յ�ң�����ź�
//			{	
//				Lost_counter = 0;
//				Control=0x0e;	 
//				MotorControl();
//			}
//		}
		INRecv();
	}
}
