/************************************************************************
�����������

		�ŷ�����Ŀ�����ͨ��һ�θ�����ĳ��������Ƶģ� 1.5msֹͣ��1.7msȫ����
����1.3msȫ�ٷ�ת��������֮��Ϊһ��20ms���ҵĵ͵�ƽ��
		������ͨ������speedleft��speedright�����ƶ�ʱ����ʱ��ȣ������Ե�ִ��
speedleft����---�͵�ƽ��ʱ---speedright����---�͵�ƽ��ʱ,�Ӷ�ʵ�ֲ�����ʱ
�߾��ȵĿ��Ƶ��ת�١�
************************************************************************/
#include "motor.h"
#include "INRecv.h"
#define MAX_SPEED 200
sbit left=P1^0;	   //��������
sbit right=P1^1;	 //��������
/*����ѭ�����Ŷ��� 1Ϊ���ߣ�0Ϊ����*/
sbit INHL=P3^6;
sbit INHR=P3^7;
sbit INBM=P3^5;

unsigned char Motor_counter=0;								//����������ѭ������
unsigned char IN_counter=0;										//����ѭ��������
unsigned int speedleft=1382,speedright=1382;	//1382*1.08507=1.5ms
unsigned char speed_H=0xff;										//TH1 temp
unsigned char speed_L=0x00;										//TL1 temp
int Rv=0,Lv=0;													//�������-185~185 ��ת-ֹͣ-����
int Rv_temp,Lv_temp;											//Rv temp��Lv temp

void MotorControl(void)											//�����յ������ݴ���ɵ���ٶȿ�����
{
	//7��ʾֹͣ,0��f��ʾȫ��ת
	Lv_temp = ((Control/16) - 7) * 30;
	Rv_temp = ((Control&0x0f) - 7) * 30;

//	int X_temp,Y_temp;													
//	X_temp=Control/16;
//	Y_temp=Control&0x0f;				  //����X��Y����
//	X_temp-=7;
//	Y_temp-=7;
//	Lv_temp=(X_temp-Y_temp)*26;
//	Rv_temp=(X_temp+Y_temp)*26;									//ת���ٶ�
}
void InitTimer1(void)
{
	TMOD |= 0x10;
    TH1 = 0x0DC;
    TL1 = 0x00;
    EA = 1;
    ET1 = 1;
    TR1 = 1;
	PT1=1;
}
void Timer1Interrupt(void) interrupt 3
{
	speedleft=1382+Lv;
	speedright=1382-Rv;			//ת���ٶ�ֵ
	TH1=speed_H;						//ʹ����һ�μ���õĶ�ʱ�����װ��ֵ
	TL1=speed_L;
	if(IN_counter>0)IN_counter--;		//����ѭ����ʱ
	Motor_counter++;									  
	if(Motor_counter>7)Motor_counter=0;
	Lost_counter++;						//ң������ʱ����

	if(IN_counter==0)					//����ѭ����ʱ����
	{
		if(INHR==1&&INHL==1&&INBM==1)
		{
			Rv=Rv_temp;
			Lv=Lv_temp;						//ң���ٶ�
		}
		else
		{
			if(INHR==0)						//��
			{
				Lv=-MAX_SPEED;
				Rv=MAX_SPEED;				//��ת
				IN_counter=80;
			}
			if(INHL==0)						//��
			{
				Lv=MAX_SPEED;
				Rv=-MAX_SPEED;				//��ת
				IN_counter=80;
			}
			if(INBM==0)						//��
			{
				Lv=MAX_SPEED;
				Rv=MAX_SPEED;				//ǰ��
				IN_counter=100;
			}
		}
	}	
	switch(Motor_counter)
	{
		case 0:								//Left������
			speed_H=0xf1;				//Ϊ��һ������(�͵�ƽ��ʱ)װ��ʱ����ֵ
			speed_L=0x9a;
			left=1;
		break;
		case 1:								//�͵�ƽ��ʱ
		case 2:								//ΪʹTime1���и�׼ȷ����ʱ���ã����͵�ƽ��ʱ��Ϊ�������
		case 3:
			speed_H=(0xffff-speedright)>>8;
			speed_L=(0xffff-speedright)&0x00ff;
			left=0;
		break;
		case 4:								//Right������
			speed_H=0xf1;
			speed_L=0x9a;
			right=1;
		break;
		case 5:								//�͵�ƽ��ʱ
		case 6:	
		case 7:			
			speed_H=(0xffff-speedleft)>>8;
			speed_L=(0xffff-speedleft)&0x00ff;
			right=0;
		break;
		default:break;	
	}
}

