#include "INRecv.h"
#include "uart.h"
#include "motor.h"
unsigned short IrValue[6];
unsigned char Time;
unsigned char Control=0x77;
unsigned int Lost_counter=0;   	//ң�������ӳ�ʱ������
bit INRecv_flag=0;				//����ң�����źű�־λ,1������յ�,0����δ���յ�
sbit IRIN=P3^2;
void DelayMs(unsigned int x)   	//0.014ms
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++);
	}
}
void INT0_Init()
{
	IT0=1;       	//�½��ش���
	EX0=1;
	PX0=0;
	EA=1;
}

void INT_0() interrupt 0
{
	INRecv_flag=1;	//���ձ��
}
void INRecv(void)
{
	unsigned int j,k;
	unsigned int err;
	EX0=0;
	INRecv_flag=0;
	Time=0;	
//	DelayMs(70);
//	if(Lost_counter>2000)
//		Control=0xee;
	
	if(IRIN==0)			//ȷ���Ƿ���Ľ��յ���ȷ���ź�
	{	 
		printf("IRAN begin!!\r\n");
		err=850;				//1000*10us=10ms,����˵�����յ�������ź�
		/*������������Ϊ����ѭ���������һ������Ϊ�ٵ�ʱ������ѭ������ó�������ʱ
		�������������*/	
		while((IRIN==0)&&(err>0))	//�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ  		
		{			
			DelayMs(1);	//0.014ms
			err--;
		} 
		if(IRIN==1)					//�����ȷ�ȵ�9ms�͵�ƽ
		{
			err=500;
			while((IRIN==1)&&(err>0))	//�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
			{
				DelayMs(1);
				err--;
			}
			for(k=0;k<1;k++)		//����4������
			{				
				for(j=0;j<8;j++)	//����һ������
				{

					err=60;		
					while((IRIN==0)&&(err>0))	//�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
					{
						DelayMs(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	//����ߵ�ƽ��ʱ�䳤�ȡ�
					{
						DelayMs(1);//0.014ms
						Time++;
						err--;
						if(Time>300)	  
						{
							printf("Time is too long !!\r\n");
							EX0=1;
							return;
						}
					}
					IrValue[k]>>=1;	 	//k��ʾ�ڼ�������
					if(Time>=8)			//����ߵ�ƽ���ִ���565us����ô��1
					{
						IrValue[k]|=0x80;
					}
					Time=0;				//����ʱ��Ҫ���¸�ֵ							
				}
				//printf("======== the %d data %x ======= \r\n", k, IrValue[k]);
			}
		}
//		if(IrValue[2]!=~IrValue[3])
//		{
//			printf("Data is error !!\r\n");
//			EX0=1;
//			return;
//		}
		printf("IrValue[0]: %x\n", IrValue[0]);
		if (IrValue[0] == 0x8d) {
			Control = 0xee; //ǰ��
		} else if (IrValue[0] == 0x2b) {
			Control = 0x00;//����
		} else if (IrValue[0] == 0x87) {
			Control = 0xe0;//��ת
		} else if(IrValue[0] == 0x89) {
			Control = 0x0e;//��ת
		} else if(IrValue[0] == 0x81) {
			Control = 0x77;//ֹͣ
		} else {
		   	Control = 0xee;//�����-ǰ��
		}

		//Control=IrValue[0];		 //�����յ������ݵ�������洢����,�����������
		Lost_counter=0;
		printf("Control : %x\r\n",Control);
	}
	MotorControl();
	EX0=1;
}