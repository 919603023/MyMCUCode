#include <stdio.h>
#include <Wire.h>
#include <stdio.h>
#include <stdlib.h>
#include "LiquidCrystal_I2C.h"
#define DHT11PIN 2
#define _SS_MAX_RX_BUFF 128 // RX buffer size
#define RELAY_PIN 7
#define LIGHT_PIN A2
#define HUORE A1
#define ALARM 6
#define buzzer 6
//实例化一个对象并设置LCD1602设备地址，这里的地址是0x3F，一般是0x20，或者0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
String comdata;
int MySerial_Save(int type);
int Serial_Find(String comdata, char *Back);
int MySerial_Save(int type);
int Make_Sms(char *buf);
int Send_Sms(char *sms, int len);
int Get_Light(char *buf) ;
void Make_Tcp(char *buf);
int Send_Tcp(char *buf);
void Alarm();
void Get_Env();
void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(RELAY_PIN, OUTPUT); 
  timerIsr();
}
void loop()
{

    
  if(MySerial_Save(2) >= 0)
  {

    if (Serial_Find(comdata, "open") == 0)
    {
      digitalWrite(RELAY_PIN,HIGH);
      Serial.println("继电器已开启");
    } 
    if (Serial_Find(comdata, "close") == 0)
    {
      digitalWrite(RELAY_PIN,LOW);
      Serial.println("继电器已关闭");
    }
    if (Serial_Find(comdata, "buzzerop") == 0)
    {
      
      Serial.println("蜂鸣器已报警");
    }
    if (Serial_Find(comdata, "envop") == 0)
    {
      
      Serial.println("环境自动检测已开启");
    }
     if (Serial_Find(comdata, "envcl") == 0)
    {
      
      Serial.println("环境自动检测已关闭");
    }
     if (Serial_Find(comdata, "smssend") == 0)
    {
      
      Serial.println("短信已发出");
    }
    if (Serial_Find(comdata, "smsget") == 0)
    {
      
      Serial.println("已收到获取信息短信");
    }
    if (Serial_Find(comdata, "smsop") == 0)
    {
      
      Serial.println("已收到控制信息短信");
    }
    if (Serial_Find(comdata, "tcpsend") == 0)
    {
      
      Serial.println("TCP信息已发出");
    }
    if (Serial_Find(comdata, "tcpget") == 0)
    {
      
      Serial.println("已收到TCP信息");
    }
    comdata = "";
  }            
}
void timerIsr() //定时器中断处理函数
{

  int Hum,Temp;
  Hum = 37;
  Temp = 19;
  lcd.clear();
  lcd.setCursor(0, 0);
  char buf[20] = "";
  sprintf(buf, "%d%%/%d", Hum, Temp);
  lcd.print(buf);
  lcd.print((char)223);
  lcd.print('C');
    lcd.setCursor(14, 0);
    lcd.print("OK");
  lcd.setCursor(8, 1);
  
  lcd.print("Normal");
}

int Serial_Find(String comdata, char *Back)
{
  if (comdata.indexOf(Back) != -1)
    return 0;
  
  return -1;
}
int MySerial_Save(int type)
{
  
    if (Serial.available())
    {
      while (Serial.available() > 0)
      {
        comdata += char(Serial.read());
        delay(2);
      }
      if (comdata.length() > 0)
      {
        return 1;
      }
    }
    return -1;
  
}

int Get_Light(char *buf) //获取当前亮度信息，
{
  if (analogRead(LIGHT_PIN) > 700)
  {
    sprintf(buf, "%s", "TooLight");
    return 2;
  }
  else if (analogRead(LIGHT_PIN) < 150)
  {
    sprintf(buf, "%s", "TooDark");
    return 0;
  }
  else
  {
    sprintf(buf, "%s", "Normal");
    return 1;
  }
}


void Alarm()
{
  unsigned long nowtime = millis(); 
  while (millis() > nowtime+5000)
 {
  int i;
  for(i=0;i<80;i++) 
    {
      digitalWrite(buzzer,HIGH);
      delay(1);
      digitalWrite(buzzer,LOW);
      delay(1);
    }
 } 
}
