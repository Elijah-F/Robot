

/************************************************************************
�����������

		�ŷ�����Ŀ�����ͨ��һ�θ�����ĳ��������Ƶģ� 1.5msֹͣ��1.7msȫ����
����1.3msȫ�ٷ�ת��������֮��Ϊһ��20ms���ҵĵ͵�ƽ��
		������ͨ������speedleft��speedright�����ƶ�ʱ����ʱ��ȣ������Ե�ִ��
speedleft����---�͵�ƽ��ʱ---speedright����---�͵�ƽ��ʱ,�Ӷ�ʵ�ֲ�����ʱ
�߾��ȵĿ��Ƶ��ת�١�
************************************************************************/

#include "motor.h"
#include "HCSR04.h"
sbit left  = P1^0;	//��������
sbit right = P1^1;	//��������

unsigned char Motor_counter=0;					//����������ѭ������
unsigned char IN_counter=0;						//����ѭ��������
unsigned int speedleft=1382,speedright=1382;	//1382*1.08507=1.5ms	(1382+180)*1.08507=1.7ms	(1382-180)*1.08507=1.3ms
unsigned char speed_H=0xff;						//TH1 temp
unsigned char speed_L=0x00;						//TL1 temp
unsigned int HCT_counter=0;						//������������
unsigned char IN_flag=0;						//����ѭ�����λ 'R','L'
unsigned char HCT_flag=0;						//HCT_flag����������⵽�ϰ���
int Rv=0,Lv=0;
		
												//�������-185~185 ��ת-ֹͣ-����
void InitTimer1(void)     
{
	TMOD |= 0x10;		 //0001 0000  ��ʽ1 16λ��ʱ��ģʽ
    TH1 = 0xDC;			 //11.0592M������10ms,10ms��ʱ��1���,�Դ�֮��ʱ��1�����ʱ�佫������10ms
    TL1 = 0x00;
    EA  = 1;
    ET1 = 1;
    TR1 = 1;
	PT1 = 1;			 //��ʱ��1����Ϊ�����ȼ�
}
void Timer1Interrupt(void) interrupt 3
{
	speedleft = 1382 + Lv;
	speedright = 1382 - Rv;		//ת���ٶ�ֵ
	TH1 = speed_H;				//ʹ����һ�μ���õĶ�ʱ�����װ��ֵ
	TL1 = speed_L;
	if(IN_counter>0) IN_counter--;			//����ѭ����ʱ
	if(HCT_counter>0) HCT_counter--;			//��������ʱ

	/*�������*/
	Motor_counter++;									
	if(Motor_counter > 7)
		Motor_counter = 0;


	//�˴�ͨ�������峤�ȿ��ƶ��ת��
	switch(Motor_counter)
	{
		case 0:
			left = 1;	 						//Left������
			speed_H = 0xf1;					//Ϊ��һ������(�͵�ƽ��ʱ)װ��ʱ����ֵ
			speed_L = 0x9a;					//���������ֵ37114
			break;
		case 1:								//�͵�ƽ��ʱ
		case 2:								//ΪʹTime1���и�׼ȷ����ʱ���ã����͵�ƽ��ʱ��Ϊ�������
		case 3:
			left=0;
			speed_H = (0xffff - speedright) >> 8;		   //speed_H = speedright��8λ	
			speed_L = (0xffff - speedright) & 0x00ff;	   //speed_L = speedright��8λ	�������ֵΪ speedright
			break;
		case 4:								//Right������
			speed_H = 0xf1;
			speed_L = 0x9a;
			right = 1;
			break;
		case 5:								//�͵�ƽ��ʱ
		case 6:	
		case 7:			
			speed_H = (0xffff - speedleft) >> 8;
			speed_L = (0xffff - speedleft) & 0x00ff;
			right=0;
			break;
		default:break;	
	}
}

