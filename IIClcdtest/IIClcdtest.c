#include <reg52.h>
#include <intrins.h>
sbit SCL = P2^0;
sbit SDA = P2^1;
bit ack;
sbit RS=P2^0;
sbit Rw=P2^1;
sbit E =P2^2;
sbit button1 =P1^0;

unsigned char LCD_data;
unsigned char code digit[ ]={"0123456789"}; //�����ַ�������ʾ����
/*
  bt0=date&0x01;
  bt1=date&0x02;
  bt2=date&0x04;
  bt3=date&0x08;
  bt4=date&0x10;
  bt5=date&0x20;
  bt6=date&0x40;
  bt7=date&0x80;
*/
//*****************��ʱ************************
void delay_nus(unsigned int n) //N us��ʱ����
{
	unsigned int i=0;
		for (i=0;i<n;i++)
			_nop_();
}
void delay_nms(unsigned int n) //N ms��ʱ����
{
	unsigned int i,j;
		for (i=0;i<n;i++)
			for (j=0;j<1140;j++);
}

void nop4()
{
	 _nop_();     //�ȴ�һ����������
	 _nop_();     //�ȴ�һ����������
	 _nop_();     //�ȴ�һ����������
	 _nop_();     //�ȴ�һ����������
}
//***************************************88
void Start()
{
 	SDA=1;
    _nop_();
    SCL=1;
	nop4();
    SDA=0;
	nop4();
    SCL=0;
    _nop_();
	_nop_();
}

void Stop()
{
 	SDA=0;
    _nop_();	

	SCL=0;
	nop4();//>4us��SCL����
	SCL=1;
	nop4();
	SDA=1;
    _nop_();
    _nop_();
}
//******************************************
void  Write_A_Byte(unsigned char c)
{
 unsigned char BitCnt;
  for(BitCnt=0;BitCnt<8;BitCnt++)  //Ҫ���͵����ݳ���Ϊ8λ
    {
     if((c<<BitCnt)&0x80)  SDA=1;   //�жϷ���λ
     else  SDA=0;                
     _nop_();
     SCL=1;               //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
     nop4(); 
     _nop_();       
     SCL=0; 
    }  
    _nop_();
    _nop_();
    SDA=1;               //8λ��������ͷ������ߣ�׼������Ӧ��λ
    _nop_();
    _nop_();  
    SCL=1;
    _nop_();
    _nop_();
    _nop_();
    if(SDA==1)ack=0;     
       else ack=1;        //�ж��Ƿ���յ�Ӧ���ź�
    SCL=0;
    _nop_();
    _nop_();
}

bit Write_Random_Address_Byte(unsigned char add,unsigned char dat)
{
 	Start();    //��������
	Write_A_Byte(add); //����������ַ
    if(ack==0)return(0);
	Write_A_Byte(dat);   //��������
    if(ack==0)return(0);
	Stop(); //��������
    return(1);
}
//********************Һ����ʹ��*********************
void Enable_LCD_write()
{
    LCD_data|=(1<<(3-1));//E=1;
    Write_Random_Address_Byte(0x40,LCD_data);
    delay_nus(2);
    LCD_data&=~(1<<(3-1));//E=0;
    Write_Random_Address_Byte(0x40,LCD_data);
}

//*************д����****************************
void LCD_write_command(unsigned char command)
{
	delay_nus(16);
	LCD_data&=~(1<<(1-1));//RS=0;
	LCD_data&=~(1<<(2-1));//RW=0;
    Write_Random_Address_Byte(0x40,LCD_data);

	LCD_data&=0x0f; //�����λ
	LCD_data|=command & 0xf0; //д����λ
    Write_Random_Address_Byte(0x40,LCD_data);
    Enable_LCD_write();

	command=command<<4; //����λ�Ƶ�����λ
	LCD_data&=0x0f; //�����λ
	LCD_data|=command&0xf0; //д����λ
    Write_Random_Address_Byte(0x40,LCD_data);
    Enable_LCD_write();
}
//*************д����****************************
void LCD_write_data(unsigned char value) 
{
	delay_nus(16);
	LCD_data|=(1<<(1-1));//RS=1;
	LCD_data&=~(1<<(2-1));//RW=0;
    Write_Random_Address_Byte(0x40,LCD_data);

	LCD_data&=0X0f; //�����λ
	LCD_data|=value&0xf0; //д����λ
    Write_Random_Address_Byte(0x40,LCD_data);
    Enable_LCD_write();

	value=value<<4; //����λ�Ƶ�����λ
	LCD_data&=0x0f; //�����λ
	LCD_data|=value&0xf0; //д����λ
    Write_Random_Address_Byte(0x40,LCD_data);
    Enable_LCD_write();
}

//**********************������ʾλ��*********************************
void set_position(unsigned char x,unsigned char y)
{
	unsigned char position;
	if (y == 0)
        position = 0x80 + x;
	else 
		position = 0xc0 + x;
    LCD_write_command(position);
}
//**********************��ʾ�ַ���*****************************

void display_string(unsigned char x,unsigned char y,unsigned char *s)
{ 
    set_position(x,y);
	while (*s) 
 	{     
		LCD_write_data(*s);     
		s++;     
 	}
}
//********************��ʾ����*******************************xs
void display_num(unsigned char x,unsigned char y,unsigned int num)
{ 
   unsigned char i,j,k,m,n;
   set_position(x,y);
    i=num/10000;
    j=num/1000%10; 
	k=num/100%10;
	m=num/10%10;
    n=num%10;
	LCD_write_data(digit[i]);    //����λ���ֵ��ַ�����д��LCD
	LCD_write_data(digit[j]);    //��ǧλ���ֵ��ַ�����д��LCD
	LCD_write_data(digit[k]);
	LCD_write_data(digit[m]);
    LCD_write_data('.');
	LCD_write_data(digit[n]);
}
//*************Һ����ʼ��****************************
void LCD_init(void) 
{ 
	LCD_write_command(0x28);
	delay_nus(40); 
	LCD_write_command(0x28);
	delay_nus(40); 
    Enable_LCD_write();
	delay_nus(40);
	LCD_write_command(0x28); //4λ��ʾ������������������������������������
	LCD_write_command(0x0c); //��ʾ��
	LCD_write_command(0x01); //����
	delay_nms(2);
}
//
//********************����*********************
void button()
{
    if(button1==0)
    {
    LCD_data|=(1<<(4-1));//E=1;
    Write_Random_Address_Byte(0x40,LCD_data);
    }
    if(button1==1)
    {
    LCD_data&=~(1<<(4-1));//E=0;
    Write_Random_Address_Byte(0x40,LCD_data);
    }
}
void main(void)
{
	int i;
	LCD_init();
	display_string(0,0,"Hello Today!"); //��ʾһ������
	while(1)
	{ 
        button();
		display_num(0,1,i); 
		delay_nms(50);
		i++;
	}
}