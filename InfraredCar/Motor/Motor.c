/************************************************************************
�����������

	�ŷ�����Ŀ�����ͨ��һ�θ�����ĳ��������Ƶģ� 1.5msֹͣ��1.7msȫ����
����1.3msȫ�ٷ�ת��������֮��Ϊһ��20ms���ҵĵ͵�ƽ
	������ͨ������speedleft��speedright�����ƶ�ʱ����ʱ��ȣ������Ե�ִ��
speedleft����---�͵�ƽ��ʱ---speedright����---�͵�ƽ��ʱ,�Ӷ�ʵ�ֲ�����ʱ
�߾��ȵĿ��Ƶ��ת��
************************************************************************/
#include "motor.h"
sbit left=P1^0;	   	//��������
sbit right=P1^1;	//��������
unsigned char Motor_counter=0;					//����������ѭ������
unsigned int IN_counter=0;						//����ѭ��������
unsigned int speedleft=1382,speedright=1382;	//1382*1.08507=1.5ms
unsigned char speed_H=0xff;						//TH1 temp
unsigned char speed_L=0x00;						//TL1 temp
unsigned int INT_counter=0;						//������ϼ�����
unsigned char IN_flag=0;						//����ѭ�����λ 'R','L'
unsigned char INT_flag=0;						//INT_flag��������ϼ�⵽�ϰ���
int Rv=0,Lv=0;									//�������-185~185 ��ת-ֹͣ-����
void InitTimer1(void)
{
	TMOD |= 0x10;
	TH1 = 0x0DC;
	TL1 = 0x00;
	EA  = 1;
	ET1 = 1;
	TR1 = 1;
	PT1 = 1;
	P1  = 0xff;
	P3  = 0xff;
	PT1=1;
}
void Timer1Interrupt(void) interrupt 3
{
	speedleft=1382+Lv;
	speedright=1382-Rv;	
	TH1=speed_H;					
	TL1=speed_L;
	if(IN_counter>0)IN_counter--;		
	if(INT_counter>0)INT_counter--;		
	/*�������*/
	Motor_counter++;									
	if(Motor_counter>7)Motor_counter=0;
	switch(Motor_counter)
	{
		case 0:						
			speed_H=0xf1;			
			speed_L=0x9a;
			left=1;
		break;
		case 1:						
		case 2:						
		case 3:
			speed_H=(0xffff-speedright)>>8;
			speed_L=(0xffff-speedright)&0x00ff;
			left=0;
		break;
		case 4:							
			speed_H=0xf1;
			speed_L=0x9a;
			right=1;
		break;
		case 5:							
		case 6:	
		case 7:			
			speed_H=(0xffff-speedleft)>>8;
			speed_L=(0xffff-speedleft)&0x00ff;
			right=0;
		break;
		default:break;	
	}
}

