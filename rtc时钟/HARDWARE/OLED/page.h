#ifndef _PAGE_H
#define _PAGE_H

//����ǹ���olde��ʾҳ����ļ�������ҳ�桢�ؼ��ṹ���
//ֻ�ǹ�����һ��ҳ��Ļ������
#include "sys.h"
#include "oled.h"
#include "key.h"

#define ShowStyle_normal 0      //����
#define ShowStyle_hifglight 1   //����


typedef struct Page
{ 
    u8 id;          //ҳ��id    
    u8* txt;        //����һ��ҳ�������к��ֶ�Ӧ���±�
    u8* num;        //����ÿһ���ַ����ĺ�������
    u8* num2;       //num�������ֵ�ۼ�  �ۼ�ֵ��������ʾ,������������2����Ϊ0, 2. 4������
    u8* coorX;      //��������ַ���������
    u8* coorY;  
    u8 ContrNum;    //�ؼ�����
    u8 choices;     //ҳ���ṩ�Ľ����ؼ�����  С�ڵ���num    
}Page;

void SetTime(void);     //����ʱ��
void Page_main(Page* page); //��ҳ��
void ShowPage(Page* page);  

#endif
