
#include <stdio.h>
#include "dht11.h"
#include "SoftwareSerial.h"
#define DHT11PIN 2
#include <Wire.h>

#include <stdlib.h>
#include "LiquidCrystal_I2C.h"
#define _SS_MAX_RX_BUFF 128 // RX buffer size
#define LED_PIN 6
#define RELAY_PIN 5
#define LIGHT_PIN A2
SoftwareSerial mySerial(3,4 );

dht11 DHT11;
unsigned long ThisDisplay = 2000, NextDisplay = 2000;
//实例化一个对象并设置LCD1602设备地址，这里的地址是0x3F，一般是0x20，或者0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
String comdata = "";
char *NumBuf[] = {"0030", "0031", "0032", "0033", "0034", "0035", "0036", "0037", "0038", "0039"};
char *RH_TM_Buf = "5EA64E3AFF1A";                               //中文  度为:  的编码
char buff[] = "0891683110900805F011000D91688146255844F50008AA"; //短信中心的编码//后面加数据长度和数据即可

//0891683110908705F0040D91688146255844F50008022011413452230873AF58834FE1606F
char _16[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

int GSM_Init_Flag = 0;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  GSM_init();
  lcd.init();
  lcd.backlight();

  pinMode(RELAY_PIN, OUTPUT); //5引脚为继电器
  pinMode(LED_PIN, OUTPUT);   //6引脚为LED灯

}
void loop()
{
  /*****显示刷新********************/
   LCD_Display();
  /*********************结束********************/
  /********软串口接收信息*************************/
  if (MySerial_Save(1) >= 0)
  { 
    Serial.println(comdata); //转发串口信息
    if (Serial_Find(comdata, "+CMT") == 0) // 收到短信
    {
      if (Serial_Find(comdata, "73AF58834FE1606F") == 0) //收到  环境信息
      {

        char buf[300] = "";
        Send_Sms(buf, Make_Sms(buf)); //发送短信
      }
      else if (Serial_Find(comdata, "5F00706F") == 0) //开灯
      {
        digitalWrite(LED_PIN, HIGH);
      }
      else if (Serial_Find(comdata, "5173706F") == 0) //关灯
      {
        digitalWrite(LED_PIN, LOW);
      }
      else if (Serial_Find(comdata, "5F00753598CE6247") == 0) //开电风扇
      {
        digitalWrite(RELAY_PIN, HIGH);
      }
      else if (Serial_Find(comdata, "5173753598CE6247") == 0) //关电风扇
      {
        digitalWrite(RELAY_PIN, LOW);
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
    else if (Serial_Find(comdata, "flush") == 0) //收到TCP消息
    {
      //发送TCP数据包
    }
   

    comdata = ""; //清除缓冲区
  }
  /*********结束***********************/

  if (MySerial_Save(2) >= 0)
  {
    mySerial.println(comdata); //转发串口信息
    if (Serial_Find(comdata, "send") == 0) //串口调试：发送SEND发送短信
    {
      
                    //清空缓冲区
        char buf[300] = "";
        Send_Sms(buf, Make_Sms(buf)); //发送短信
    }
    
      comdata = "";
            
  }

}
void LCD_Display() //Lcd屏幕显示函数
{
  unsigned long nowtime = millis();

  if (nowtime > ThisDisplay) //检测系统运行时间长度是否到1000ms
  {
    ThisDisplay = nowtime;        //记录当前时间长度，第一次为1000ms,赋值给ThisDisplay
    NextDisplay = nowtime + 2000; //计算出下一次led灯变化的时刻，第一次运行程序时应该在1000ms时关灯

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
}
void GSM_init() //GSM初始化函数
{
  while (Second_AT_Command("AT\n", "OK", 3000) != 0)
  {
    delay(100);
  }
  delay(10000);
  //if (Serial_Init_Find("Ready", 200) > 0)
    GSM_Init_Flag = 1;Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"\n", "OK", 3000);
    
  Second_AT_Command("ATE0\n", "OK", 3000);
  Second_AT_Command("AT+CNMI=3,2,0,0,0\n", "OK", 3000);
  Second_AT_Command("AT+CMGF=0\n", "OK", 3000);
  
  delay(100);
 // Second_AT_Command("AT+CIPSTART=\"TCP\",\"129.211.89.48\",8000", "OK", 3000);
}
int Second_AT_Command(char *AT, char *Back, unsigned int OutTime) //发送AT指令
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
int Serial_Find(String comdata, char *Back) //在string中查找指定字符串
{
  if (comdata.indexOf(Back) != -1)
  {
    return 0;
  }
  return -1;
}

int MySerial_Save(int type) //串口数据保存1为软串口2为硬串口
{
  comdata = "";
  if (type == 1)
  {
    
      while (mySerial.available())
      {
        comdata += char(mySerial.read());
        delay(2);
      }
      if (comdata.length() > 0)
      {
        return 1;
      }
  }
  else if (type == 2)
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
    
   
  }return -1 ;
}
int Send_Sms(char *sms, int len) //发送短信函数
{
  
  char buf[12] = "";
  sprintf(buf, "AT+CMGS=%d\n", len);
  Serial.println(sms);Serial.println(len);
  if (Second_AT_Command(buf, ">", 3000) == 0)
  {
  
    mySerial.write(sms);
    mySerial.write(0x1a);
  }
}

int Make_Sms(char *buf) //制作环境检测的短信包
{

  char TM[9] = "";
  char RH[9] = "";
  if (DHT11.humidity / 10 == 0) //制作温湿度信息包
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
  char Buf[10] = "";int a=Get_Light(Buf);
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
    strcpy(Buf, "8FC74EAE");
  }
  char BUF[6] = "";
  if (digitalRead(RELAY_PIN))

    strcpy(BUF, "5F00");

  else
    strcpy(BUF, "5173");
  char bufc[200] = "";
  sprintf(bufc, "6E7F%s%s6E29%s%s51497167FF1A%s706F003A%s", RH_TM_Buf, RH, RH_TM_Buf, TM, Buf, BUF);
  int tmp = strlen(bufc) / 2;
  char bufp[10] = "";
  sprintf(bufp, "%c%c", _16[tmp / 16], _16[tmp % 16]);

  sprintf(buf, "%s%s%s", buff, bufp, bufc);
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
