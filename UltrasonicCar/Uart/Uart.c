

#include "uart.h"
#include "motor.h"


/*
* ����������ʱ��������ʹ��
* ���嶨ʱ��2��T2����Ϊ�����ʷ�����
* 9600bps  11.0592MHz
*/
void Uart_Init()  //��ʱ��2��Ϊ�����ʷ�����
{
	TCLK=1;		  //ѡ��ʱ��2��Ϊ���з��Ͳ����ʷ����� 
	RCLK=1; 	  //ѡ��ʱ��2��Ϊ���н��ܲ����ʷ�����
				  //T2CON  T2��ʱ���Ĵ���
	

	RCAP2H=0xFF;  
	RCAP2L=0xDC;  //9600bps  11.0592MHz

	SCON=0x50; 	  //���пڿ��ƼĴ���	0101 0000 10λ�첽�շ�(8λ����)	�����пڽ�������
				  //D0 D1 ����/�����жϱ�־λ  �ڲ�Ӳ��ֵ1  �����������
				  //��ʽ1   TXD-P3.1    RXD-P3.0

	PCON=0x00; 	  //��Դ����Ĵ���

	TR2=1;		  //������־-����T2
	EA=1;
	ET2=1;		  //ʹ��
	PS=0;		  //���п��ж϶���Ϊ�����ȼ��ж�
}


/*
* �жϷ�����
*/
void Usart() interrupt 4
{
	unsigned char receiveData;
	receiveData=SBUF; 
	RI = 0;       //��������жϱ�־λ
}



/*
* 
*/
char putchar(char c)	//putchar���أ���ֱ�ӵ���printf����ʹ��scanf������getchar
{  
	hal_uart_putchar(c);  
	return c;  
}  


/*
*
*/
void hal_uart_putchar(char i) 
{  
	ES = 0;  		//�رմ����ж� 
	TI = 0; 		//��������жϱ�־λ
	SBUF = i;   
	while(TI == 0);	//�ȴ��жϷ������жϷ���TIӲ����1 
	TI = 0;   		//��������жϱ�־λ
	ES = 1;  		//���������ж�
}  







