#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <string.h>
#include <reg51.h>
sfr AUXR = 0X8E;
unsigned char *content=" 0011000D91685111181394F40008C4044F60597D";
/************串口相关****************/
#define Buf1_Max 200 					  //´®¿Ú2»º´æ³¤¶È
//´®¿Ú1·¢ËÍ»Ø³µ»»ÐÐ
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)
											 
											 
 void Uart1Init(void)	;	//9600bps@11.0592MHz
 void UART1_SendData(unsigned char c);//串口发送一个字符
 void UART1_SendString(char const *s);//串口发送一个字符串
 int Find(unsigned char *s);//在缓冲区查找是否有对应字符
 void CLR_Buf1(void);//清除缓冲区
 void delay_ms(unsigned char ms);
/*************************************/
#endif