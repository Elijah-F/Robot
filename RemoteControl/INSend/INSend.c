#include "INSend.h"
#define k 2
sbit OUT=P2^1; 
static bit OP; //���ⷢ��ܵ����� 
static unsigned int count; //��ʱ������ 
static unsigned int endcount; //��ֹ��ʱ���� 
static unsigned char flag; //���ⷢ�ͱ�־ 
char iraddr1; //ʮ��λ��ַ�ĵ�һ���ֽ� 
char iraddr2; //ʮ��λ��ַ�ĵڶ����ֽ� 
char senddat[5]=0x55; 
void delay1ms(unsigned int i) 
{ 
	unsigned char j; 
	while(i--) 
	{ 
		for(j=0;j<115;j++); //1ms��׼��ʱ���� 
	} 
} 
void InitTimer0(void)
{
	TMOD = 0x01;
	TH0 = 0x0FF;
	TL0 = 0x0F4;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
void INSend_Init(void)
{
	InitTimer0();
	count = 0;
	flag = 0; 
	OP = 0; 
	OUT = 0;  
	iraddr1=0x00; 
	iraddr2=0x01; 
}
void SendIRdata(char p_irdata) 
{ 
	int i; 
	char irdata; //����9ms����ʼ�� 
	endcount=346*k; 
	flag=1; 
	count=0; 
	do{}while(count<endcount); //����4.5ms�Ľ���� 
	endcount=173*k ; 
	flag=0; 
	count=0; 
	do{}while(count<endcount); 
	//����ʮ��λ��ַ��ǰ��λ 
	irdata=iraddr1; 
	for(i=0;i<8;i++) 
	{ //�ȷ���0.56ms��38KHZ���Ⲩ����������0.56ms�ĵ͵�ƽ�� 
		endcount=21*k; 
		flag=1; 
		count=0; 
		do
		{}while(count<endcount); //ֹͣ���ͺ����źŪ��������еĸߵ�ƽ�� 
		//if(irdata&0X01) //�ж϶���������λΪ1����0 
		if(irdata-(irdata/2)*2) 
		{ 
			endcount=65*k; //1Ϊ��ĸߵ�ƽ 
		} 
		else 
		{ 
			endcount=21.5*k; //0Ϊխ�ĸߵ�ƽ 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 
	//����ʮ��λ��ַ�ĺ��λ 
	irdata=iraddr2; 
	for(i=0;i<8;i++) 
	{ 
		endcount=21.5*k; 
		flag=1; 
		count=0; 
		do{}while(count<endcount); 
		// if(irdata&0X01) 
		if(irdata-(irdata/2)*2) 
		{ 
			endcount=21.5*k; 
		} 
		else 
		{ 
			endcount=21.5*k; 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 

	//���Ͱ�λ���� 
	irdata=p_irdata; 
	for(i=0;i<8;i++) 
	{ 
		endcount=21.5*k; 
		flag=1; 
		count=0; 
		do{}while(count<endcount); 
		if(irdata-(irdata/2)*2) 
		//if(irdata&0X01) 
		{ 
			endcount=65*k; 
		} 
		else 
		{ 
			endcount=21.5*k; 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 
	//���ͺ��λ���� 
	irdata=~p_irdata; 
	for(i=0;i<8;i++) 
	{ 
		endcount=21.5*k; 
		flag=1; 
		count=0; 
		do{}while(count<endcount); 
		if(irdata-(irdata/2)*2) 
		{ 
		endcount=65.5*k; 
			} 
		else 
		{ 
			endcount=21.5*k; 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 
	endcount=65*k; 
	flag=1; 
	count=0; 
	do{}while(count<endcount); 
	flag=0; 
} 
void DelayUs2x(unsigned char t) 
{ 
	while(--t); 
} 

void DelayMs(unsigned char t) 
{ 
	while(t--) 
	{ 
		//������ʱ1mS 
		DelayUs2x(245*k); 
		DelayUs2x(245*k); 
	} 
}  
void timeint(void) interrupt 1 
{ 
	TH0 = 0x0FF;
	TL0 = 0x0F4;;
	count++; 
	if(flag) 
	OUT=~OUT; 
} 

/*void delay() 
{ 
	int i,j; 
	for(i=0;i<400;i++) 
		for(j=0;j<100;j++); 
	 
}*/ 
