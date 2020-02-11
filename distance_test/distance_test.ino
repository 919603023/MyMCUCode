//LingShun lab

#include<stdio.h>
#include "TimerOne.h"
#include"dht11.h"


#include"SoftwareSerial.h"
dht11 DHT11;  
#define DHT11PIN 2 
#include <Wire.h> 
#include <stdio.h>
#include "LiquidCrystal_I2C.h" 
SoftwareSerial mySerial(4, 3);
int ledStatus = 1;
unsigned long ledOn=2000,ledOff=2000; 
//实例化一个对象并设置LCD1602设备地址，这里的地址是0x3F，一般是0x20，或者0x27
LiquidCrystal_I2C lcd(0x27,16,2);  
String comdata = "";
char *NumBuf[] = {"0030","0031","0032","0033","0034","0035","0036","0037","0038","0039"};
char  *RH_TM_Buf = "5EA64E3AFF1A";
//0891683110908705F0040D91688146255844F50008022011413452230873AF58834FE1606F

void setup()
{
  Serial.begin(9600);
mySerial.begin(9600);GSM_init();    
  lcd.init();                  
  lcd.backlight();  
         
  //lcd.setCursor(0,0);                //设置显示位置为第一行第一个  
  //lcd.setCursor(0,1);                //设置显示位置为第二行第一个
//Timer1.initialize(2000000);//初始化定时器为1s
 // Timer1.attachInterrupt( timerIsr );//设置中断回调函数
} 
void loop()
{
  unsigned long nowtime=millis(); //获取当前的系统运行时间长度
    if(ledStatus==1)             //如果当前lled灯状态为高电平，则执行该程序
    {
      if(nowtime>ledOn)             //检测系统运行时间长度是否到500ms
      {
        ledOn=nowtime;              //记录当前时间长度，第一次为500ms,赋值给ledOn
        ledOff=nowtime+2000;         //计算出下一次led灯变化的时刻，第一次运行程序时应该在1000ms时关灯
            timerIsr();  //关掉led灯
        ledStatus=0;              //记录当前led灯状态，下一次使用     
      } 
    }
 
  else{       
         if(nowtime>ledOff)
         {   
          ledOff=nowtime;
          ledOn=nowtime+2000;
          timerIsr();
          ledStatus=1;
         }
      }
   if( Serial_Save() >= 0)
   {
    if(Serial_Find("+CMT") == 0)
    {
       if(Serial_Find("73AF58834FE1606F") == 0)
       {
        Send_Sms(NULL,35);
       }
    }
    Serial.println(comdata);
    comdata = "";
   }
    if (mySerial.available())

    {
   // Serial.write(comdata);
    
      
    }
    
  if (Serial.available())
    mySerial.write(Serial.read());
    
  
}
 void timerIsr()//定时器中断处理函数
{
  
  int chk = DHT11.read(DHT11PIN);
    lcd.clear();
    lcd.setCursor(0,0);
    char buf[20] ="";
    sprintf(buf,"%d/%dC",DHT11.humidity,DHT11.temperature);
    lcd.print(buf);
}
void GSM_init()
{
  while(Second_AT_Command("AT\n","OK",3000)!=0)
  {
    delay(100);
  }
  Second_AT_Command("ATE0\n","OK",3000);
  Second_AT_Command("AT+CNMI=3,2,0,0,0\n","OK",3000);
   
Second_AT_Command("AT+CMGF=0\n","OK",3000);
//Second_AT_Command("AT+CMGS=35\n",">",3000);
//mySerial.write("0891683110900805F011000D91688146255844F50008AA144F604ED659884E86969458C176845FEB70B9505A");
//mySerial.write(0x1a);
}
int Second_AT_Command( char *AT, char *Back,unsigned int OutTime)
{
  mySerial.write(AT);
 if( Serial_Init_Find(Back,OutTime) < 0)
 {
    Serial.println("超时"); 
    return -1;
 }
   return 0;
}
int Serial_Init_Find(char *Back,unsigned int OutTime)
{
  unsigned long nowtime=millis();
  while(millis()< nowtime+OutTime)
  {
   if (mySerial.available())

    { 
      while (mySerial.available() > 0)  
    {
        comdata += char(mySerial.read());
        delay(2);
    }
    if (comdata.length() > 0)
    {
      if(comdata.indexOf(Back)!=-1)
      {
       Serial.println("收到预定目标");
        return 0;
      }
        Serial.println(comdata);
        comdata = "";
    }
      
    } 
  }
  return -1; 
}
int Serial_Find(char *Back)
{
  
 
   
      if(comdata.indexOf(Back)!=-1)
      {
       Serial.println("收到预定目标");
       Serial.write(Back);
        return 0;
      }   
  
  return -1; 
}
int Serial_Save()
{
  if (mySerial.available())

    { 
      while (mySerial.available() > 0)  
    {
        comdata += char(mySerial.read());
        delay(2);
    }
    if (comdata.length() > 0)
    {
        return 1;
        
    }
      
    }
    return -1;
}

int Send_Sms(char *sms,int len)
{
  char buf[12]="";
  sprintf(buf,"AT+CMGS=%d\n",len);
  if(Second_AT_Command(buf,">",3000) == 0)
  {
    mySerial.write("0891683110900805F011000D91688146255844F50008AA144F604ED659884E86969458C176845FEB70B9505A");
    mySerial.write(0x1A);
  }
}
