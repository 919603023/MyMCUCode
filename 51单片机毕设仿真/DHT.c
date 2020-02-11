
#include<reg52.h>
#include<intrins.h>
#include"DHT.h"
#define uchar unsigned char
#define uint  unsigned int
#define IODATA  P1
//?????????
sbit E=P3^7;    //1602????
sbit RW=P3^6;    //1602????   
sbit RS=P3^5;    //1602??/??????


/********************************************************************
* ?? : delay()
* ?? : ??
* ?? : ?
* ?? : ?
***********************************************************************/

void delay1602()
{
_nop_();
_nop_();
}

void Delay1602(uint del)
{
uint i,j;
for(i=0;i<del;i++)
for(j=0;j<=148;j++)
{
}   
}
/********************************************************************
* ?? : bit Busy(void)
* ?? : ??????????,???????????
* ?? : ??????
* ?? : ?
***********************************************************************/
void Busy(void)
{
bit busy_flag = 1;
IODATA  = 0xff;
RS = 0;
delay1602();
RW = 1;
delay1602();
E = 1;
//Delay(1);
while(1)
{
busy_flag = (bit)(IODATA  & 0x80);
if(busy_flag == 0)
{
break;
}
}
E = 0;
}
/********************************************************************
* ?? : wcmd(uchar del)
* ?? : 1602????
* ?? : ??????
* ?? : ?
***********************************************************************/
void wcmd(uchar del)
{
RS = 0;
delay1602();
RW = 0;
delay1602();
E = 0;
delay1602();
        IODATA  = del;
delay1602();
E = 1;
delay1602();
E = 0;
}

/********************************************************************
* ?? : wdata(uchar del)
* ?? : 1602?????
* ?? : ????1602???
* ?? : ?
***********************************************************************/

void lcd1602_write_data(uchar del)
{
Busy();      
delay1602();
RS = 1;
delay1602();
RW = 0;
delay1602();
E = 0;
delay1602();
IODATA  = del;
        delay1602();
E = 1;
delay1602();
E = 0;
}

/********************************************************************
* ?? : L1602_init()
* ?? : 1602???,???1602???
* ?? : ?
* ?? : ?
***********************************************************************/
void lcd1602_init(void)
{
Delay1602(15);
wcmd(0x38);
Delay1602(5);
wcmd(0x38);
Delay1602(5);
wcmd(0x38);
wcmd(0x38);
Busy();
wcmd(0x08);
Busy();   
wcmd(0x01);
Busy();
wcmd(0x06);
Busy();
wcmd(0x0c);
}

/********************************************************************
* ?? : L1602_char(uchar hang,uchar lie,char sign)
* ?? : ?????????,???????,???????"b" ,???????
L1602_char(1,5,'b')
* ?? : ?,?,????1602???
* ?? : ?
***********************************************************************/
void lcd1602_char(uchar hang,uchar lie,char sign)
{
uchar a;
if(hang == 1)
{
a = 0x80;
}
if(hang == 2)
{
a = 0xc0;
}
a = a + lie - 1;
Busy();
wcmd(a);
Busy();
lcd1602_write_data(sign);
}

/********************************************************************
* ?? : L1602_string(uchar hang,uchar lie,uchar *p)
* ?? : ?????????,???????,?????????"ab cd ef" ,???????
L1602_string(1,5,"ab cd ef;")
* ?? : ?,?,????1602???
* ?? : ?
***********************************************************************/
void lcd1602_string(uchar hang,uchar lie,uchar *p)
{
uchar a;
if(hang == 1)
{
a = 0x80;
}
if(hang == 2)
{
a = 0xc0;
}
a = a + lie - 1;
while(1)
{
Busy();
wcmd(a);
Busy();
lcd1602_write_data(*p);
a++;
p++;   
if((*p == '\0')||(a==0x90)||(a==0xd0))
{
break;
}
}
}
//????
void lcd1602_locate(unsigned char x,unsigned char y)
{ unsigned char aa;
if (x==1) aa=0x80;
if (x==2) aa=0x80+0x40;
Busy();
wcmd(aa+y-1);
Busy();
}
