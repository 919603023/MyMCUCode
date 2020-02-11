#include<reg52.h>    //����ͷ�ļ��������Ͳ��ö�˵�ˡ��� 
#include<stdio.h>
#define uint unsigned int  //Ԥ����һ�� 
#define uchar unsigned char 
sbit rs=P3^5;    //1602������/ָ��ѡ������� 
sbit rw=P3^6;        //1602�Ķ�д������ 
sbit en=P3^7;        //1602��ʹ�ܿ����� 
/*P2�ڽ�1602��D0~D7��ע�ⲻҪ�Ӵ���˳������ǰ����������Թ�����*/ 
uchar  table[]="LCD1602ddk"; //Ҫ��ʾ������1��������tablel 
uchar  table1[]="study up";             //Ҫ��ʾ������2��������table1 
uchar table2[]={"0123456789"};
void delay(uint n)       //��ʱ����                       
{ 
    uint x,y;  
    for(x=n;x>0;x--) 
        for(y=110;y>0;y--); 
}     
void lcd_wcom(uchar com)  //1602д�����                 
{ 
    rs=0;            //ѡ��ָ��Ĵ��� 
    rw=0;            //ѡ��д 
    P2=com;            //������������P2 
    delay(5);            //��ʱһС�������1602׼���������� 
    en=1;           //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ� 
    en=0; 
} 
void lcd_wdat(uchar dat)        //1602д���ݺ���       
{ 
    rs=1;        //ѡ�����ݼĴ��� 
    rw=0;        //ѡ��д 
    P2=dat;        //��Ҫ��ʾ����������P2 
    delay(5);        //��ʱһС�������1602׼���������� 
    en=1;        //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ� 
    en=0; 
} 
void lcd_init()              //1602��ʼ������       
{ 
    lcd_wcom(0x38);       //8λ���ݣ�˫�У�5*7����       
    lcd_wcom(0x0c);      //������ʾ�����ع�꣬��겻��˸ 
    lcd_wcom(0x06);    //��ʾ��ַ��������дһ�����ݺ���ʾλ������һλ 
    lcd_wcom(0x01);    //���� 
} 
void main()            //������ 
{     
		
    uchar n,m=0; 
		P1 = 0x00;
    lcd_init();                 //Һ����ʼ�� 
    lcd_wcom(0x80);   //��ʾ��ַ��Ϊ80H����00H�������ŵ�һλ       
    for(m=0;m<sizeof(table)/sizeof(uchar);m++)     //��table[]�е���������д��1602��ʾ 
    { 
            lcd_wdat(table[m]);           
            delay(20); 
    } 
    lcd_wcom(0x80+0x44); //�����趨��ʾ��ַΪ0xc4,�����ŵ�5λ 
    for(n=0;n<8;n++)   //��table1[]�е���������д��1602��ʾ 
    {     
            lcd_wdat(table1[n]); 
           delay(20); 
    } 
		lcd_wdat(table2[5]);
    while(1);        //��̬ͣ�� 
} 