

#define uchar unsigned char
#define uint  unsigned int
void lcd1602_init(void);
void lcd1602_char(uchar hang,uchar lie,char sign);
void lcd1602_string(uchar hang,uchar lie,uchar *p);
void lcd1602_write_data(uchar del);
void lcd1602_locate(unsigned char x,unsigned char y);

