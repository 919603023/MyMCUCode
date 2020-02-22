#include <stdio.h>
#include "dht11.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <stdio.h>
#include <stdlib.h>
#include "LiquidCrystal_I2C.h"
#define DHT11PIN 2
#define _SS_MAX_RX_BUFF 128 // RX buffer size
#define RELAY_PIN 5
#define LIGHT_PIN A2
#define HUORE A1
#define ALARM 6
SoftwareSerial mySerial(4, 3);
dht11 DHT11;
int ledStatus = 1;
int GSM_Init_Flag = 0;
int GET_ENV_Flag = 0;
unsigned long ledOn = 2000, ledOff = 2000;
//实例化一个对象并设置LCD1602设备地址，这里的地址是0x3F，一般是0x20，或者0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
String comdata = "";

char *NumBuf[] = {"0030", "0031", "0032", "0033", "0034", "0035", "0036", "0037", "0038", "0039"};
char *RH_TM_Buf = "5EA64E3AFF1A";                               //中文  度为:  的编码
char buff[] = "0891683110900805F011000D91688146255844F50008AA"; //短信中心的编码//后面加数据长度和数据即可

//0891683110908705F0040D91688146255844F50008022011413452230873AF58834FE1606F
char _16[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  GSM_init();
  lcd.init();
  lcd.backlight();
  pinMode(RELAY_PIN, OUTPUT); 
}
void loop()
{
  unsigned long nowtime = millis(); 
  if (ledStatus == 1)               
  {
    if (nowtime > ledOn) 
    {
      ledOn = nowtime;         
      ledOff = nowtime + 2000; 
      timerIsr();              
      ledStatus = 0;           
    }
  }
  else
  {
    if (nowtime > ledOff)
    {
      ledOff = nowtime;
      ledOn = nowtime + 2000;
      timerIsr();
      ledStatus = 1;
    }
  }
  if (MySerial_Save(1) >= 0)
  {
    if (Serial_Find(comdata, "+CMT") == 0)
    {
      if (Serial_Find(comdata, "73AF58834FE1606F") == 0)
      {
        //组包
        char buf[300] = "";
        Send_Sms(buf, Make_Sms(buf));
      }
      else if (Serial_Find(comdata, "5F00706F") == 0) //开灯
      {
        digitalWrite(RELAY_PIN, HIGH);
      }
      else if (Serial_Find(comdata, "5173706F") == 0) //关灯
      {
        digitalWrite(RELAY_PIN, LOW);
      }
      else if(Serial_Find(comdata, "5F00542F68C06D4B6A215F0F") == 0)//开启检测模式
      {
        GET_ENV_Flag = 1;
      }
      else if(Serial_Find(comdata, "5F00542F68C06D4B6A215F0F") == 0)//关闭检测模式
      {
         GET_ENV_Flag = 0;
      }
    }
    else if (Serial_Find(comdata, "close") == 0)
    {
      digitalWrite(LED_PIN, LOW);  
    }
    else if (Serial_Find(comdata, "open") == 0)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else if (Serial_Find(comdata, "flush") == 0) 
    {
      char *buf[100] = "";
      Send_Tcp(buf);
    }
    else if(Serial_Find(comdata,"Call") == 0)
    {
      GSM_Init_Flag = 1;
      Second_AT_Command("AT+CIPSTART=\"TCP\",\"129.211.89.48\",8000", "OK", 3000);
      delay(1000);
      Second_AT_Command("AT+CIPSTART=\"TCP\",\"129.211.89.48\",8000", "OK", 3000);
      delay(1000);
      Second_AT_Command("AT+CIPSTART=\"TCP\",\"129.211.89.48\",8000", "OK", 3000);
    }
    else if (Serial_Find(comdata, "GET_ENV_Flag1") == 0) 
    {
      GET_ENV_Flag = 1;
    }
    else if (Serial_Find(comdata, "GET_ENV_Flag0") == 0) 
    {
      GET_ENV_Flag = 0;
    }
    Serial.println(comdata);
    comdata = "";
  }
  if (MySerial_Save(2) >= 0)
  {

    if (Serial_Find(comdata, "send") == 0)
    {
      char buf[300] = "";

      Send_Sms(buf, Make_Sms(buf));
    }
    mySerial.println(comdata);
    comdata = "";
  }
  Get_Env();
}
void timerIsr() //定时器中断处理函数
{

  int chk = DHT11.read(DHT11PIN);
  lcd.clear();
  lcd.setCursor(0, 0);
  char buf[20] = "";
  sprintf(buf, "%d%%/%d", DHT11.humidity, DHT11.temperature);
  lcd.print(buf);
  lcd.print((char)223);
  lcd.print('C');
  if (GSM_Init_Flag == 1)
  {
    lcd.setCursor(14, 0);
    lcd.print("OK");
  }
  lcd.setCursor(8, 1);
  char Buf[10] = "";
  Get_Light(Buf);
  lcd.print(Buf);
}
void GSM_init()
{
  while (Second_AT_Command("AT\n", "OK", 3000) != 0)
  {
    delay(100);
  }
  Second_AT_Command("ATE0\n", "OK", 3000);
  Second_AT_Command("AT+CNMI=3,2,0,0,0\n", "OK", 3000);
  Second_AT_Command("AT+CMGF=0\n", "OK", 3000);
  Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"\n", "OK", 3000);
}
int Second_AT_Command(char *AT, char *Back, unsigned int OutTime)
{
  mySerial.write(AT);
  if (Serial_Init_Find(Back, OutTime) < 0)
  {
    Serial.println("超时");
    return -1;
  }
  return 0;
}
int Serial_Init_Find(char *Back, unsigned int OutTime)
{
  unsigned long nowtime = millis();
  while (millis() < nowtime + OutTime)
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
        if (comdata.indexOf(Back) != -1)
        {
          Serial.print("收到");
          Serial.println(Back);
          return 0;
        }
        Serial.println(comdata);
        comdata = "";
      }
    }
  }
  return -1;
}
int Serial_Find(String comdata, char *Back)
{
  if (comdata.indexOf(Back) != -1)
    return 0;
  }
  return -1;
}
int MySerial_Save(int type)
{
  if (type == 1)
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
  else if (type == 2)
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
}
int Send_Sms(char *sms, int len)
{
  char buf[12] = "";
  sprintf(buf, "AT+CMGS=%d\n", len);
  if (Second_AT_Command(buf, ">", 3000) == 0)
  {
    buf[strlen(buf)] = '\0';
    mySerial.write(sms);
    mySerial.write(0x1a);
  }
}

int Make_Sms(char *buf)
{
  char TM[9] = "";
  char RH[9] = "";
  if (DHT11.humidity / 10 == 0)
  {
    sprintf(RH, "%s", NumBuf[DHT11.humidity % 10]);
  }
  else
  {
    sprintf(RH, "%s%s", NumBuf[DHT11.humidity / 10], NumBuf[DHT11.humidity % 10]);
  }
  if (DHT11.temperature / 10 == 0)
  {
    sprintf(TM, "%s", NumBuf[DHT11.temperature % 10]);
  }
  else
  {
    sprintf(TM, "%s%s", NumBuf[DHT11.temperature / 10], NumBuf[DHT11.temperature % 10]);
  }
  char Buf[10] = "";
  int a=Get_Light(Buf);
  if (a == 1)
  {
    strcpy(Buf, "6B635E38");
  }
  else if (a > 1)
  {
    strcpy(Buf, "8FC74EAE");
  }
  else
  {
    strcpy(Buf, "8FC76697");
  }
   char BUFF[10 ] = "";
  if(analogRead(HUORE) > 600)
    strcpy(BUFF,"517395ED");
  else 
    strcpy(BUFF,"62535F00");

  char BUF[6] = "";
  if (digitalRead(RELAY_PIN))
    strcpy(BUF, "5F00");
  else
    strcpy(BUF, "5173");
  
  char bufc[200] = "";
  sprintf(bufc, "6E7F%s%s00206E29%s%s002051497167FF1A%s002095E8FF1A%s0020706F003A%s", RH_TM_Buf, RH, RH_TM_Buf, TM, Buf, BUFF,BUF);
  int tmp = strlen(bufc) / 2;
  char bufp[10] = "";
  sprintf(bufp, "%c%c", _16[tmp / 16], _16[tmp % 16]);

  sprintf(buf, "%s%s%s", buff, bufp, bufc);
  Serial.println(tmp);
  Serial.write(_16[tmp / 16]);
  Serial.write(_16[tmp % 16]);

  Serial.println((String(buf).length() - 18) / 2);
  return (strlen(buf) - 18) / 2;
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
void Make_Tcp(char *buf)
{
   char Buf[5] = "";
  int a=Get_Light(Buf);
  if (a == 1)
  {
    strcpy(Buf, "N");
  }
  else if (a > 1)
  {
    strcpy(Buf, "L");
  }
  else
  {
    strcpy(Buf, "D");
  }
  char BUFF[5] = "";
  if(analogRead(HUORE) > 600)
    strcpy(BUFF,"N");
  else 
    strcpy(BUFF,"D");
  char BUF[6] = "";
  if (digitalRead(RELAY_PIN))
    strcpy(BUF, "open");
  else
    strcpy(BUF, "close");
  sprintf(buf,"%d:%d:%s:%s:%s",DHT11.humidity,DTH11.temperature,Buf,BUFF,BUF);
}
int Send_Tcp(char *buf)
{
  if (Second_AT_Command("AT+CIPSEND", ">", 3000) == 0)
  {
    mySerial.write(buf);
    mySerial.write(0x1a);
  }
}
void Alarm()
{
  unsigned long nowtime = millis(); 
  while (millis() > nowtime+5000)
 {
  for(i=0;i<80;i++) 
    {
      digitalWrite(buzzer,HIGH);
      delay(1);
      digitalWrite(buzzer,LOW);
      delay(1);
    }
 } 
}

void Get_Env()
{
  char buf[15] = "";
  if(GET_ENV_Flag  == 1)
  {
    if((analogRead(HUORE) < 600) || (Get_Light(buf) != 1) )
    {
      Alarm();
      char buf[300] = "";
      Send_Sms(buf, Make_Sms(buf));
    }
  }
}