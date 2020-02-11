/**********************************************************************************
 * ������  :����绰
 * ����    :ͨ��C51���������ģ�鲦��ָ���绰��
 * ʵ��ƽ̨:C51
 * ��汾  :


 * Ӳ������˵��
	 ʹ�õ�Ƭ������GPRSģ��ͨ�� 
	 C51        GPRSģ��
	 P30 (RXD)->RXD
	 P31 (TXD)->TXD
	 GND	    ->GND

 * �������˵��
   �����ϵ������ָʾ��RUNING_LED����һ���Ƶ����˸
	 �򿪴����޸Ķ������ĺš����շ��ֻ��źͶ������ݣ�����ɹ������ص���Ƭ�����棬
	 �Ϳ���ʵ�ַ���һ�����Ķ��ţ����Ķ��ž���Ľ������Բο����ڵ��Աʼ�
**********************************************************************************/
#include "config.h"
#include "string.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "intrins.h"
#define Buf1_Max 200 					  //����2���泤��
/*************	���س�������	**************/
sbit RUNING_LED = P3^2;					//����ָʾ��
sbit io=P1^0;//dht11data�˽ӵ�Ƭ����P1^0��// 

sbit rw=P3^6;//һ������������lcd1602��ʹ�ܶ�// 

sbit rs=P3^5; 

sbit ep=P3^7; 

typedef bit BOOL;//������һ�������ͱ���������// 

unsigned char data_byte; 

unsigned char RH,RL,TH,TL; 
//code unsigned char *content="0891683108706505F011000D91683197985889F90008AA0C6D4B8BD55DF27ECF5B8C6210";//���Ͷ�������
code unsigned char *content="0891683110900805F011000D91688146255844F50008AA144F604ED659884E86969458C176845FEB70B9505A";//���Ͷ�������
code char buff[] ="0891683110900805F011000D91683197985889F90008AA";
code char *NumBuf[] = {"0030","0031","0032","0033","0034","0035","0036","0037","0038","0039"};
code char  *RH_TM_Buf = "5EA64E3AFF1A";
/************************�������ݽ���**************************************/
/*
   //�������ĺţ�����       Ŀ���ֻ�������             �������ݣ������Ѿ���ɣ�
     8613800756500F           8613798985989F          
0891 683108706505F0 1100 0D91 683197985889F9 0008AA   0C   6D4B8BD55DF27ECF5B8C6210
       ������  683110900805F0                                            �ֽ���
																										 
																									14	 4F604ED659884E86969458C176845FEB70B9505A 
*/
/*************  ���ر�������	**************/

xdata u8 Uart1_Buf[Buf1_Max];

u8 Times=0,First_Int = 0,shijian=0,Second_2s,Uart_flag;//Uart_flagΪ����flagΪ1 ʱ��û����Ϊ0ʱ�����

bdata u8 Flag;//��ʱ����־λ
sbit Timer0_start =Flag^0;	//��ʱ��0��ʱ����������



/*************	���غ�������	**************/
void GPIO_config(void); //�˿ڳ�ʼ������
void Timer0Init(void);  //��ʱ��0��ʼ��
void CLR_Buf1(void);    //�����\��2���ջ���
u8 Find(u8 *a);         //�����ַ���
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time); //����ATָ���
void Set_Pdu_Mode(void);//���ö���ģʽPDU
void Wait_CREG(void);   //��ѯ�ȴ�ģ��ע��ɹ�
void Send_Pdu_Sms(unsigned char *sms,int len);//����һ�����Ķ���
void DHT_display();
void receive();
unsigned char receive_byte();
void display(unsigned char addr,unsigned char q);
void start();
void lcd_init();
void write_byte(unsigned char dat);
void write_addr(unsigned char addr);
void write_cmd(unsigned char cmd);
BOOL lcd_bz();
void delay1();
void delay(unsigned char ms);
 void Delay1s() ;
 void Check_New_Message(void);
 void Back_Send_Sms();
void Send_Sms(char *sms,int len);

/*************  �ⲿ�����ͱ�������*****************/




/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : ���ڲ�������9600��GPRSģ��Ĭ�ϲ�������115200����Ҫ�Լ�ͨ�����������޸�
				   Ϊ9600����ʹ�á� 
*******************************************************************************/
void main(void)
{
	Timer0Init();  //��ʼ����ʱ��0
	//GPIO_config();
	EA=1;	//�����ж�
	
Uart1Init();    //��ʼ������9600
Wait_CREG();    //��ѯ�ȴ�ģ��ע��ɹ
	
	Send_Pdu_Sms(content,35);//����һ������Ϣ
	
	while(1)
	{
		if(Second_2s < 2)
		{
				//DHT_display();
			
		}
		if(Uart_flag == 1)
		{
		//0.	Check_New_Message();
		}
	}
	
}
/*******************************************************************************
* ������ : Check_Message_rec
* ����   : ����Ƿ�������Ϣ����ִ����Ϣ����ָ��
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Check_New_Message(void)
{
	u8 temp=0;
	if(strstr(Uart1_Buf,"+CMT")!=NULL)   		//�������ַ����к���"+CMT"�ͱ�ʾ���µĶ���
	{
		delay_ms(3);//�ȴ�����ȫ���������
		if(strstr(Uart1_Buf,"73AF58834FE1606F")!=NULL)//��ȡ������Ϣ
		{
		//���÷��Ͷ��ź���
			Back_Send_Sms();
		}
		else if(strstr(Uart1_Buf,"5173706F")!=NULL)//�ص�
		{
		//
		}
		else if(strstr(Uart1_Buf,"5F00706F")!=NULL)//����
		{
		//
		}
   	else if(strstr(Uart1_Buf,"62535F00753598CE6247")!=NULL)//�򿪵����
		{
		//
		} 
		else if(strstr(Uart1_Buf,"517395ED753598CE6247")!=NULL)//�򿪵����
		{
		//
		} 
		CLR_Buf1();
		//Second_AT_Command("AT+CMGD=1,4","OK",3);//ɾ������
	}
}


/*******************************************************************************
* ������ : Uart1 
* ����   : ����1�жϷ�����ں���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Uart1() interrupt 4
{
    if (RI)
    {
      RI = 0;                 //���RIλ
			Uart1_Buf[First_Int] = SBUF;  	  //�����յ����ַ����浽������
			First_Int++;                			//����ָ������ƶ�
			if(First_Int > Buf1_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
    }
		
}
/*******************************************************************************
* ������ : Timer0_ISR
* ����   : ��ʱ��0�жϷ�����ں���,20ms�ж�һ��
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Timer0_ISR() interrupt 1
{
	static u8 Time_count=0;
  TL0 = 0x00;		  //���ö�ʱ����ֵ
	TH0 = 0xB8;		  //���ö�ʱ����ֵ	
	TR0=0;//�ض�ʱ��
	
	Time_count++;
	if(Time_count>=50)
	{
		Time_count = 0;
		RUNING_LED =~RUNING_LED;
	}
	
	Second_2s++;
	if(Second_2s >= 100)
	{
		Second_2s = 0;
		
	}
	
	if(count_20ms) //20ms��ʱ������
		count_20ms--;
	if(Timer0_start)
	Times++;
	if(Times > (50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	TR0=1;//����ʱ��
}
/*******************************************************************************
* ������ : GPIO_config
* ����   : IO�����ú���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void	GPIO_config(void)
{
		RUNING_LED=0;
}
/*******************************************************************************
* ������ : Timer0Init
* ����   : ��ʱ��0��ʼ����20ms��ʱ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Timer0Init(void)		//20����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//
	TMOD |= 0x01;		//���ö�ʱ��ģʽ��16λ��ʱ��
	TL0 = 0x00;		  //���ö�ʱ����ֵ
	TH0 = 0xB8;		  //���ö�ʱ����ֵ
	TF0 = 0;		    //��TF0��־
	TR0 = 1;		    //��ʱ��0��ʼ��ʱ
	ET0 = 1;    	  //ʹ�ܶ�ʱ��0�ж�
}
/*******************************************************************************
* ������ : CLR_Buf1
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //��������������
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
	Uart_flag = 0;
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(u8 *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(u8 *b,u8 *a,u8 wait_time)         
{
	u8 i;
	u8 *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a))           //������ҪӦ����ַ�
		{
			if(Timer0_start == 0)//��ʱ���·�������
			{
				b = c;						 //���ַ�����ַ��b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;  //��ʼ��ʱ
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;  
		}
	}
	CLR_Buf1(); 
}

/*******************************************************************************
* ������ : Set_Pdu_Mode
* ����   : ���ö���ΪPDU�ı�ģʽ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_Pdu_Mode(void)
{
	Second_AT_Command("ATE0","OK",3);										  //ȡ������	
	Second_AT_Command("AT+CNMI=3,2,0,0,0","OK",3);							//�¶���ֱ�����
	Second_AT_Command("AT+CMGF=0","OK",3);								//����PDUģʽ	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//���в�������SIM���н���
}
/*******************************************************************************
* ������ : Send_Pdu_Sms
* ����   : ����PDU�ı�����
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_Pdu_Sms(unsigned char *sms,int len)
{
	char buf[20] = "";
	sprintf(buf,"AT+CMGS=%d",len);
	Second_AT_Command(buf,">",3); //�������ݳ��ȣ�27������ļ��㷽�������ڵ��ԱȽϣ����յ���>���ŷ��Ͷ�������
	UART1_SendString(sms);     //���Ͷ�������
	UART1_SendData(0X1A);          //���ͽ�����
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf1();
  while(i == 0)        			
	{
		CLR_Buf1();        
		UART1_SendString("AT+CREG?");//��ѯģ������ע��״̬
		UART1_SendLR();
		delay_ms(250);  						
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+4] == '1')||(Uart1_Buf[k+4] == '5')) //��������ע��ɹ�
				{
					i = 1;
				  break;
				}
			}
		}
	}
}




/***********************************************/


 void Delay1s()                //@11.0592MHz
{
        unsigned char i, j, k;

        _nop_();
        i = 8;
        j = 1;
        k = 243;
        do
        {
                do
                {
                        while (--k);
                } while (--j);
        } while (--i);
}

//***************��ʱ����************************************* 

 void delay(unsigned char ms) //��ʱģ��// 

{ 

  unsigned char i; 

  while(ms--)                 

        

 for(i=0;i<100;i++); 

} 


void delay1()//һ��forѭ�������Ҫ8����������ڪ�һ����������Ϊ1us������Ϊ12MHz����Ҳ����˵��������ʱ8us�ડ����ʱ�����������΢��ȷһ�㪢 

{ 

  unsigned char i; 

  for(i=0;i<1;i++); 

} 

  

//***************************************************************  

//lcdģ��// 

BOOL lcd_bz()//����lcdæµ״̬������ֵΪ��������ֵ�����٪�'1'.'0'��  

{ 

 BOOL result;
  rs=0;           // ��æ�ź�
  rw=1;
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  result = (BOOL)(P2&0x80);
  ep=0;
return  result ;

}     

void write_cmd(unsigned char cmd)//дָ��// 

{ 

  while (lcd_bz());
  rs=0;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=cmd ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;

} 

 

void write_addr(unsigned char addr)//д��ַ// 

{ 

  write_cmd(addr|0x80);//LCD��һ�е��׵�ַΪ0x80���ڶ��е��׵�ַΪ0x80+0x40=0xc0 

} 

 

void write_byte(unsigned char dat) //д�ֽ�// 

{ 

   while (lcd_bz());
  rs=1;
  rw=0;
  ep=0;
  _nop_();
  _nop_();
  P2=dat ;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ep=0;

} 

 

void lcd_init() //lcd��ʼ��// 

{ 

  write_cmd(0x38);//����LCD������ʾ��һ��������5*7�����ʾ,������8���ߴ��� 

  delay(1); 

  write_cmd(0x0c);//�����Ļ��ʾ 

  delay(1); 

  write_cmd(0x06);//�趨���뷽ʽ����������λ 

  delay(1); 

  write_cmd(0x01);//��������ʾ���ع�ꪡ����˸ 

  delay(1); 

} 

 

void display(unsigned char addr,unsigned char q)//��ĳһ��ַ����ʾ���ݪ�adder��ʾ���ǵ�ַƫ������q��ʾ��ʾ���ַ�������// 

{ 

  delay(10);   write_addr(addr); 

  write_byte(q); 

  delay(1);//�޸Ĵ�ʱ�䪣���Ըı�LCD����ֵ��������� 

} 

 

//**************************dht11����ĳ��*************************************// 

void start()//��ʼ�ź� 

{ 

  io=1; 

  delay1(); 

  io=0; 

  delay(25);// �������������ͱ������18ms����֤DHT11�ܼ�⵽��ʼ�ź� 

  io=1;    //���Ϳ�ʼ�źŽ��������ߵ�ƽ��ʱ20-40us 

  delay1();//����������ʱ�������Ϊ24us������Ҫ�� 

  delay1(); 

  delay1(); 

} 

 

unsigned char receive_byte()//����һ���ֽ�// 

{ 

  unsigned char i,temp; 

  for(i=0;i<8;i++)//����8bit������ 

  { 

 	while(!io);//�ȴ�50us�ĵ͵�ƽ��ʼ�źŽ��� 

 	delay1();//��ʼ�źŽ���֮����ʱ26us-28us������������ʱ������ 

	 delay1(); 

 	delay1(); 

 	temp=0;//ʱ��Ϊ26us-28us����ʾ���յ�Ϊ����'0' 

 	if(io==1) 

	 temp=1; //���26us-28us֮�󪣻�Ϊ�ߵ�ƽ�����ʾ���յ�����Ϊ'1' 

    while(io);//�ȴ������źŸߵ�ƽ��'0'Ϊ26us-28us��'1'Ϊ70us�� 

 	data_byte<<=1;//���յ�����Ϊ��λ��ǰ�����ƪ� 

	 data_byte|=temp; 

  } 

  return data_byte; 

} 

    

void receive()//��������// 

{ 

  unsigned char T_H,T_L,R_H,R_L,check,num_check,i; 

  start();//��ʼ�ź�// 

  io=1;   //������Ϊ���몣�жϴӻ���DHT11����Ӧ�ź� 

  if(!io)//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź�// 

  {  
	while(!io);//�жϴӻ����� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����// 

	while(io);//�жϴӻ����� 80us �ĸߵ�ƽ�Ƿ������������������������ݽ���״̬ 

	R_H=receive_byte();//ʪ�ȸ�λ 

 	R_L=receive_byte();//ʪ�ȵ�λ 

	T_H=receive_byte();//�¶ȸ�λ 

	T_L=receive_byte();//�¶ȵ�λ 

	check=receive_byte();//У��λ 

	io=0; //�����һbit���ݽ���Ϻ󪣴ӻ����͵�ƽ50us// 

	for(i=0;i<7;i++)//���50us����ʱ 

	delay1(); 

	io=1;//�����������������ߪ��������״̬ 

	num_check=R_H+R_L+T_H+T_L; 

	if(num_check==check)//�ж϶������ĸ�����֮���Ƿ���У��λ��ͬ 

 	{ 

   RH=R_H; 

   RL=R_L; 

   TH=T_H; 

   TL=T_L; 

   check=num_check; 

 	} 

  } 

} 

//*****************************************************************************

 

void DHT_display()//������ģ��// 

{ 

  lcd_init();//��ʼ��LCD 

  while(1) 

  { Delay1s();Delay1s();

    receive();//�������� 

 



 display(0x00,RH/10+0x30); //0x30��ʾ�����ֿ��LCD1602��0x30��λ�÷�������0��RH/10+0x30����ʾʪ�ȵ�ʮλ�������ֿ�RH/10+0x30��λ�ô����Ū� 

 display(0x01,RH%10+0x30); 

 display(0X03,'%'); 


 display(0X04,'/'); 
 display(0x42-0x3d,TH/10+0x30); 

 display(0x43-0x3d,TH%10+0x30); 

 display(0x44-0x3d,0xdf);//�����������¶ȵ�λ�Ĵ��� 

 display(0x45-0x3d,0x43); 

  } 
} 


void Back_Send_Sms()
{
	
	xdata char buf[200]="";
	sprintf(buf,"%s6E7F%s%s%s6E29%s",buff,RH_TM_Buf,NumBuf[RH/10],NumBuf[RH%10],RH_TM_Buf);
	Send_Sms(buf,strlen(buf)-18);
}

void Send_Sms(char *sms,int len)
{
	xdata char buf[12]="";
	sprintf(buf,"AT+CMGS=%d",len);
	Second_AT_Command(buf,">",3); //�������ݳ��ȣ�27������ļ��㷽�������ڵ��ԱȽϣ����յ���>���ŷ��Ͷ�������
	UART1_SendString(sms);     //���Ͷ�������
	UART1_SendData(0X1A);          //���ͽ�����
}