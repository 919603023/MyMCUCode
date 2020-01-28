//LingShun lab
#include <Wire.h> 
#include <stdio.h>
#include "LiquidCrystal_I2C.h" 
 
//实例化一个对象并设置LCD1602设备地址，这里的地址是0x3F，一般是0x20，或者0x27
LiquidCrystal_I2C lcd(0x27,16,2);  
  char buff[] = {"123456789"};
void setup()
{
  
 
  lcd.init();                  // 初始化LCD
  lcd.backlight();             //设置LCD背景等亮
  lcd.setCursor(0,0);                //设置显示位置为第一行第一个
  lcd.print("LCD1602iic Testfdfsdf");     //输出字符串到LCD1602上
  lcd.setCursor(0,1);                //设置显示位置为第二行第一个
  lcd.print("         by L.L.");
} 
void loop()
{
  int i = 0;
  for( i = 0 ;i < sizeof(buff)/sizeof(char)-1; i++ )
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(buff+i);
    lcd.setCursor(16-i,0);
    for(int j = 0;j < i ;j++)
    {
      lcd.print(buff[j]);
    }
    delay(1000);
  }
  for(;i < 16 && 16-i >= 0;i++)
  {
    lcd.clear();
    lcd.setCursor(16-i,0);
    lcd.print(buff);
    delay(1000);
  } 
}
 
