#ifndef _OLED_H
#define _OLED_H

#include "sys.h"

#define CS    PEout(12)         //Ƭѡ
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_12)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_12)

#define RST   PDout(8)         //��λ 
#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_8)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_8)

#define DC    PEout(14)          //����/���� 0����  1����
#define OLED_DC_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_14)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOE,GPIO_Pin_14)

#define SCLK  PBout(0)          //spiʱ��   D0
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define SDIN  PDout(10)         //spi����   D1
#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_10)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_10)


#define OLED_CMD 0
#define OLED_DATA 1

//OLED�����ú���
//��ʾϵͳ
void OLED_Init(void);
void OLED_WR_Byte(u8 dat,u8 cmd);	
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);	

//snake��ʾ
void OLED_Set_Pos(unsigned char x, unsigned char y);
void XY_On(u8 x, u8 y);
void XY_Off(u8 x, u8 y);   

//��ʾ
void OLED_ShowChar(u8 x,u8 y,u8 chr, u8 style);
void OLED_ShowString(u8 x, u8 y, u8* stri, u8 style);
void OLED_ShowChinese(u8 x,u8 y,u8 no, u8 style);    //style��ʾ��ʽ 0������ʾ  1������ʾ
void OLED_ShowWord(u8 x, u8 y, u8* st, u8 len, u8 style); //��ʾ�����ı�  len������
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size, u8 mode, u8 style);
void OLED_ShowTime(u8 x, u8 y, u8 h, u8 m, u8 s);  //��ʾʱ��



#endif
