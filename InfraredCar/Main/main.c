/*******************************************************
2018�й���������������˫����̨����С������
оƬ��AT89S52
����11.0592
��Χ������	������� 		4	
			����ѭ�� 		3
			����ѹģ��	    1
********************************************************/
#include "uart.h"
#include "motor.h"
/*����ѭ�����Ŷ���*/
sbit INHL=P3^7;
sbit INHR=P3^6;
sbit INBM=P3^5;
/*����������Ŷ���*/
sbit INTHM=P3^2;
sbit INTHL=P1^2;
sbit INTHR=P1^3;
sbit INTBM=P1^4;

#define MAX_SPEED 200

void main()
{
	Uart_Init();
	InitTimer1();
	while(1)
	{	
		//��������ڳ�������˵���ò��,���Ӽ�
		if(IN_counter==0)				//����ѭ����ʱ����
		{
			if(INHR==1&&INHL==1&&INBM==1)
			{
				if(INT_counter==0)      //�з���⣬���������ʱ����
				{
					INT_flag=0;
					if(INTBM==0)    		//�󷽼�⵽���˴���
					{
						if(IN_flag=='R')	//�������ұ�������ת
						{
							Rv=MAX_SPEED;
							Lv=-MAX_SPEED;		//���������������ת
						}
						else
						{
							Rv=-MAX_SPEED;
							Lv=MAX_SPEED;
						}
						INT_counter=400;
						INT_flag++;
					}
					if(INTHR==0)  			//�ҷ���⵽���˴���
					{
						Rv=-MAX_SPEED;
						Lv=MAX_SPEED;
						INT_counter=200;
						INT_flag++;
					}
					if(INTHL==0)  			//��ż�⵽���˴���
					{
						Rv=MAX_SPEED;
						Lv=-MAX_SPEED;
						INT_counter=200;
						INT_flag++;
					}
					if(INTHM==0)			//ǰ����⵽���˴���
					{
						Rv=MAX_SPEED;
						Lv=MAX_SPEED;
						INT_flag++;
					}
					if(INT_flag==0) 		//�����δ�ܼ�⵽,ȫ��ֱ��
					{
						Rv=MAX_SPEED;
						Lv=MAX_SPEED;
					}
				}
				else
				{
					if(INTHM==0)			//ǰ��������ϼ�⵽
					{
						INT_counter=0;		//ֹͣ���������ת��ʱ	
					}
				}
			}
			else 										//���ؼ��
			{
				if(INBM == 0) 					//��
				{
					Lv=MAX_SPEED;		
					Rv=MAX_SPEED;						//ǰ��
					IN_counter=200;
				}
				if(INHL == 0)  						//��
				{
					Lv=MAX_SPEED;
					Rv=-MAX_SPEED;					//��ת
					IN_flag='L';
					IN_counter=200;	
				}
				if(INHR == 0) 						//��
				{
					Lv=-MAX_SPEED;
					Rv=MAX_SPEED;						//��ת
					IN_flag='R';
					IN_counter=200;
				}

			}
		}
		else
		{
			if(INTHM==0)						//ǰ��������ϼ�⵽
			{
				IN_counter=0;					//ֹͣ����ѭ����ת��ʱ
			}	
		}
	}
}
