/**********************************************************************************
 * 工程名  :拨打电话
 * 描述    :通过C51开发板控制模块拨打指定电话号
 * 实验平台:C51
 * 库版本  :


 * 硬件连接说明
	 使用单片串口与GPRS模块通信 
	 C51        GPRS模块
	 P30 (RXD)->RXD
	 P31 (TXD)->TXD
	 GND	    ->GND

 * 软件功能说明
   板子上电后运行指示灯RUNING_LED会以一秒的频率闪烁
	 打开代码修改短信中心号、接收方手机号和短信内容，编译成功后下载到单片机里面，
	 就可以实现发送一条中文短信，中文短信具体的解析可以参考串口调试笔记
**********************************************************************************/
#include "config.h"
#include "string.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "intrins.h"
#define Buf1_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/
sbit RUNING_LED = P3^2;					//运行指示灯
sbit io=P1^0;//dht11data端接单片机的P1^0口// 

sbit rw=P3^6;//一下三行是设置lcd1602的使能端// 

sbit rs=P3^5; 

sbit ep=P3^7; 

typedef bit BOOL;//此声明一个布尔型变量即真或假// 

unsigned char data_byte; 

unsigned char RH,RL,TH,TL; 
//code unsigned char *content="0891683108706505F011000D91683197985889F90008AA0C6D4B8BD55DF27ECF5B8C6210";//发送短信内容
code unsigned char *content="0891683110900805F011000D91688146255844F50008AA144F604ED659884E86969458C176845FEB70B9505A";//发送短信内容
code char buff[] ="0891683110900805F011000D91683197985889F90008AA";
code char *NumBuf[] = {"0030","0031","0032","0033","0034","0035","0036","0037","0038","0039"};
code char  *RH_TM_Buf = "5EA64E3AFF1A";
/************************短信内容解析**************************************/
/*
   //短信中心号（倒序）       目标手机（倒序）             短信内容（测试已经完成）
     8613800756500F           8613798985989F          
0891 683108706505F0 1100 0D91 683197985889F9 0008AA   0C   6D4B8BD55DF27ECF5B8C6210
       黑龙江  683110900805F0                                            字节数
																										 
																									14	 4F604ED659884E86969458C176845FEB70B9505A 
*/
/*************  本地变量声明	**************/

xdata u8 Uart1_Buf[Buf1_Max];

u8 Times=0,First_Int = 0,shijian=0,Second_2s,Uart_flag;//Uart_flag为处理flag为1 时，没处理，为0时处理过

bdata u8 Flag;//定时器标志位
sbit Timer0_start =Flag^0;	//定时器0延时启动计数器



/*************	本地函数声明	**************/
void GPIO_config(void); //端口初始化配置
void Timer0Init(void);  //定时器0初始化
void CLR_Buf1(void);    //清除串\口2接收缓存
u8 Find(u8 *a);         //查找字符串
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time); //发送AT指令函数
void Set_Pdu_Mode(void);//设置短信模式PDU
void Wait_CREG(void);   //查询等待模块注册成功
void Send_Pdu_Sms(unsigned char *sms,int len);//发送一条中文短信
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

/*************  外部函数和变量声明*****************/




/*******************************************************************************
* 函数名 : main 
* 描述   : 主函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 串口波特率是9600，GPRS模块默认波特率是115200，需要自己通过串口助手修改
				   为9600方可使用。 
*******************************************************************************/
void main(void)
{
	Timer0Init();  //初始化定时器0
	//GPIO_config();
	EA=1;	//开总中断
	
Uart1Init();    //初始化串口9600
Wait_CREG();    //查询等待模块注册成�
	
	Send_Pdu_Sms(content,35);//发送一条短消息
	
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
* 函数名 : Check_Message_rec
* 描述   : 检查是否有新信息，并执行信息内容指令
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Check_New_Message(void)
{
	u8 temp=0;
	if(strstr(Uart1_Buf,"+CMT")!=NULL)   		//若缓存字符串中含有"+CMT"就表示有新的短信
	{
		delay_ms(3);//等待数据全部接收完成
		if(strstr(Uart1_Buf,"73AF58834FE1606F")!=NULL)//获取环境信息
		{
		//调用发送短信函数
			Back_Send_Sms();
		}
		else if(strstr(Uart1_Buf,"5173706F")!=NULL)//关灯
		{
		//
		}
		else if(strstr(Uart1_Buf,"5F00706F")!=NULL)//开灯
		{
		//
		}
   	else if(strstr(Uart1_Buf,"62535F00753598CE6247")!=NULL)//打开电风扇
		{
		//
		} 
		else if(strstr(Uart1_Buf,"517395ED753598CE6247")!=NULL)//打开电风扇
		{
		//
		} 
		CLR_Buf1();
		//Second_AT_Command("AT+CMGD=1,4","OK",3);//删除短信
	}
}


/*******************************************************************************
* 函数名 : Uart1 
* 描述   : 串口1中断服务入口函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Uart1() interrupt 4
{
    if (RI)
    {
      RI = 0;                 //清除RI位
			Uart1_Buf[First_Int] = SBUF;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf1_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
    }
		
}
/*******************************************************************************
* 函数名 : Timer0_ISR
* 描述   : 定时器0中断服务入口函数,20ms中断一次
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Timer0_ISR() interrupt 1
{
	static u8 Time_count=0;
  TL0 = 0x00;		  //重置定时器初值
	TH0 = 0xB8;		  //重置定时器初值	
	TR0=0;//关定时器
	
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
	
	if(count_20ms) //20ms延时计数器
		count_20ms--;
	if(Timer0_start)
	Times++;
	if(Times > (50*shijian))
	{
		Timer0_start = 0;
		Times = 0;
	}
	TR0=1;//开定时器
}
/*******************************************************************************
* 函数名 : GPIO_config
* 描述   : IO口配置函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void	GPIO_config(void)
{
		RUNING_LED=0;
}
/*******************************************************************************
* 函数名 : Timer0Init
* 描述   : 定时器0初始化，20ms定时
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Timer0Init(void)		//20毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//
	TMOD |= 0x01;		//设置定时器模式，16位定时器
	TL0 = 0x00;		  //设置定时器初值
	TH0 = 0xB8;		  //设置定时器初值
	TF0 = 0;		    //清TF0标志
	TR0 = 1;		    //定时器0开始计时
	ET0 = 1;    	  //使能定时器0中断
}
/*******************************************************************************
* 函数名 : CLR_Buf1
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //将缓存内容清零
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
	Uart_flag = 0;
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(u8 *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(u8 *b,u8 *a,u8 wait_time)         
{
	u8 i;
	u8 *c;
	c = b;										//保存字符串地址到c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a))           //查找需要应答的字符
		{
			if(Timer0_start == 0)//超时重新发送命令
			{
				b = c;						 //将字符串地址给b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;  //开始计时
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
* 函数名 : Set_Pdu_Mode
* 描述   : 设置短信为PDU文本模式
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_Pdu_Mode(void)
{
	Second_AT_Command("ATE0","OK",3);										  //取消回显	
	Second_AT_Command("AT+CNMI=3,2,0,0,0","OK",3);							//新短信直接输出
	Second_AT_Command("AT+CMGF=0","OK",3);								//设置PDU模式	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//所有操作都在SIM卡中进行
}
/*******************************************************************************
* 函数名 : Send_Pdu_Sms
* 描述   : 发送PDU文本短信
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_Pdu_Sms(unsigned char *sms,int len)
{
	char buf[20] = "";
	sprintf(buf,"AT+CMGS=%d",len);
	Second_AT_Command(buf,">",3); //发送数据长度：27（具体的计算方法看串口调试比较）接收到“>”才发送短信内容
	UART1_SendString(sms);     //发送短信内容
	UART1_SendData(0X1A);          //发送结束符
}

/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
		UART1_SendString("AT+CREG?");//查询模块网络注册状态
		UART1_SendLR();
		delay_ms(250);  						
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+4] == '1')||(Uart1_Buf[k+4] == '5')) //表明网络注册成功
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

//***************延时函数************************************* 

 void delay(unsigned char ms) //延时模块// 

{ 

  unsigned char i; 

  while(ms--)                 

        

 for(i=0;i<100;i++); 

} 


void delay1()//一个for循环大概需要8个多机器周期一个机器周期为1us晶振为12MHz也就是说本函数延时8us多此延时函数必须德稍微精确一点 

{ 

  unsigned char i; 

  for(i=0;i<1;i++); 

} 

  

//***************************************************************  

//lcd模块// 

BOOL lcd_bz()//测试lcd忙碌状态返回值为布尔型数值真或假'1'.'0'  

{ 

 BOOL result;
  rs=0;           // 读忙信号
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

void write_cmd(unsigned char cmd)//写指令// 

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

 

void write_addr(unsigned char addr)//写地址// 

{ 

  write_cmd(addr|0x80);//LCD第一行的首地址为0x80第二行的首地址为0x80+0x40=0xc0 

} 

 

void write_byte(unsigned char dat) //写字节// 

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

 

void lcd_init() //lcd初始化// 

{ 

  write_cmd(0x38);//设置LCD两行显示一个数据由5*7点阵表示,数据由8跟线传输 

  delay(1); 

  write_cmd(0x0c);//清除屏幕显示 

  delay(1); 

  write_cmd(0x06);//设定输入方式增量不移位 

  delay(1); 

  write_cmd(0x01);//开整体显示关光标不闪烁 

  delay(1); 

} 

 

void display(unsigned char addr,unsigned char q)//在某一地址上显示内容adder表示的是地址偏移量q表示显示的字符或数字// 

{ 

  delay(10);   write_addr(addr); 

  write_byte(q); 

  delay(1);//修改此时间可以改变LCD上数值跳变的数度 

} 

 

//**************************dht11测试某块*************************************// 

void start()//开始信号 

{ 

  io=1; 

  delay1(); 

  io=0; 

  delay(25);// 主机把总线拉低必须大于18ms保证DHT11能检测到起始信号 

  io=1;    //发送开始信号结束后拉高电平延时20-40us 

  delay1();//以下三个延时函数差不多为24us符合要求 

  delay1(); 

  delay1(); 

} 

 

unsigned char receive_byte()//接收一个字节// 

{ 

  unsigned char i,temp; 

  for(i=0;i<8;i++)//接收8bit的数据 

  { 

 	while(!io);//等待50us的低电平开始信号结束 

 	delay1();//开始信号结束之后延时26us-28us以下三个延时函数 

	 delay1(); 

 	delay1(); 

 	temp=0;//时间为26us-28us表示接收的为数据'0' 

 	if(io==1) 

	 temp=1; //如果26us-28us之后还为高电平则表示接收的数据为'1' 

    while(io);//等待数据信号高电平'0'为26us-28us'1'为70us 

 	data_byte<<=1;//接收的数据为高位在前右移 

	 data_byte|=temp; 

  } 

  return data_byte; 

} 

    

void receive()//接收数据// 

{ 

  unsigned char T_H,T_L,R_H,R_L,check,num_check,i; 

  start();//开始信号// 

  io=1;   //主机设为输入判断从机DHT11响应信号 

  if(!io)//判断从机是否有低电平响应信号// 

  {  
	while(!io);//判断从机发出 80us 的低电平响应信号是否结束// 

	while(io);//判断从机发出 80us 的高电平是否结束如结束则主机进入数据接收状态 

	R_H=receive_byte();//湿度高位 

 	R_L=receive_byte();//湿度低位 

	T_H=receive_byte();//温度高位 

	T_L=receive_byte();//温度低位 

	check=receive_byte();//校验位 

	io=0; //当最后一bit数据接完毕后从机拉低电平50us// 

	for(i=0;i<7;i++)//差不多50us的延时 

	delay1(); 

	io=1;//总线由上拉电阻拉高进入空闲状态 

	num_check=R_H+R_L+T_H+T_L; 

	if(num_check==check)//判断读到的四个数据之和是否与校验位相同 

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

 

void DHT_display()//主函数模块// 

{ 

  lcd_init();//初始化LCD 

  while(1) 

  { Delay1s();Delay1s();

    receive();//接收数据 

 



 display(0x00,RH/10+0x30); //0x30表示带字库的LCD1602中0x30的位置放有数字0RH/10+0x30即表示湿度的十位数字在字库RH/10+0x30的位置处放着 

 display(0x01,RH%10+0x30); 

 display(0X03,'%'); 


 display(0X04,'/'); 
 display(0x42-0x3d,TH/10+0x30); 

 display(0x43-0x3d,TH%10+0x30); 

 display(0x44-0x3d,0xdf);//以下两个是温度单位的处理 

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
	Second_AT_Command(buf,">",3); //发送数据长度：27（具体的计算方法看串口调试比较）接收到“>”才发送短信内容
	UART1_SendString(sms);     //发送短信内容
	UART1_SendData(0X1A);          //发送结束符
}