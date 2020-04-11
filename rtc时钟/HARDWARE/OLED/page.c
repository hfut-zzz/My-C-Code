#include "page.h"
#include "rtc.h"
#include "key.h" 
#include "led.h"
#include "delay.h"
#include <stdlib.h>


//����oled��ʾ����
static u8 pageID = 0;
extern Page pageMain;

void SetTime()     //����ʱ��  ����һЩ���⣬һЩСbug �޸���ʱ̫��
{
    u8* val;         //��¼ʱ������ֵ
    u8 temp = 0;    //��ǰ�����޸ĵ����� ��5����4����3, ʱ2����1����0
    u8 key;
    u8 key2;    //���ڼ�ⰴ������
    u8 flag = 0;//���ڼ�ⳤ��
    u16 count2 = 0;//���ڼ�ⳤ��
    u16 count = 0;
    OLED_ShowString(5, 0, "Setting time...", 0);
    while(1)
    { 
        val = RTC_GetTime();
        if(count > 250)
            OLED_ShowNum((44 - temp * 22 + calendar.x), calendar.y, val[temp], 2, 16, 1, 1);
        else
            OLED_ShowTime(calendar.x, calendar.y, calendar.hour, calendar.min, calendar.sec);  
        key = KEY_Scan(0);
        key2 = KEY_Scan(1);
        
        if(key == KEY0_PRES)
        {
            temp++;
            if(temp >= 3)
                temp = 0;
        }     
        else if(key == WKUP_PRES)
        {
            if(temp == 0)
                temp = 3;    //��ʱֻ֧���޸�ʱ����
            temp--;
            
        }
        else if(key == KEY1_PRES)   //�����޸�
        {
            while(1)
            {
                if(count > 200)
                    OLED_ShowNum((44 - temp * 22 + calendar.x), calendar.y, val[temp], 2, 16, 1, 1);
                else 
                    OLED_ShowNum((44 - temp * 22 + calendar.x), calendar.y, val[temp], 2, 16, 1, 0);                    
                    //OLED_ShowTime(calendar.x, calendar.y, calendar.hour, calendar.min, calendar.sec);
                key = KEY_Scan(0);
                
                if(key == KEY0_PRES)
                {
                    val[temp]++;
                    if(val[temp] >= 60 || (temp == 2 && val[temp] >= 24))
                        val[temp] = 0;
                }     
                else if(key == WKUP_PRES)
                {
                    if(val[temp] == 0)
                    {
                        if(temp == 2)
                            val[temp] = 24;
                        else
                            val[temp] = 60;    //��ʱֻ֧���޸�ʱ����                        
                    }
                    val[temp]--;
                }
                else if(key == KEY1_PRES)   //�˳��޸� 
                {
                    RTC_Set(calendar.year, calendar.month, calendar.date, val[2], val[1], val[0]);
                    break;
                }                    
                                        
                count++;
                if(count >= 500)
                    count = 0;
            }
        }  
        if(key2 == KEY0_PRES)
            flag = 1;
        else 
            flag = 0;
        if(flag == 1 && key2 == KEY0_PRES)
            count2++;
        else
            count2 = 0;
        if(count2 >= 100)
        {
            OLED_Clear();
            ShowPage(&pageMain);
            break;            
        }

        
        count++;
        if(count >= 500)
        {
            count = 0; 
           LED1 = ~LED1;
        }
    }
}

u8 tx[4] = {21, 22, 23, 24};       //�˵�  ����
u8 X[2] = {0, 96};        //���꣺
u8 Y[2] = {6, 6};
u8 Num[2] = {2, 2};      //������
u8 Num2[2] = {0, 2};

//��ʼ��ҳ���ģ��
//void Page(Page* page)
//{
//    u8 tx[] = {};       //����
//    u8 X[] = {};        //���꣺
//    u8 Y[] = {};
//    u8 Num[] = {};      //������
//    u8 Num2[] = {};
//    page = (Page*)malloc(sizeof(Page));
//    page->choices = 2;
//    page->ContrNum = 2;
//    page->txt = tx;
//    page->coorX = X;
//    page->coorY = Y;
//    page->num = Num;
//    page->num2 = Num2;
//    page->id = pageID++; 
//}

void Page_main(Page* page)
{
    page->choices = 2;           //���ܰѾ��������ֵ���ṹ�壬�����������󣬾�������Ŀռ䱻�ͷţ�ָ��ָ���ֵ�Ͳ�ȷ����
    page->ContrNum = 2;
    page->txt = tx;
    page->coorX = X;
    page->coorY = Y;
    page->num = Num;
    page->num2 = Num2;
    page->id = pageID++; 
}



void ShowPage(Page* page)
{
    u8 i;
    OLED_Clear();
    
    for(i = 0; i < 2; i++)  //(page->ContrNum)
    {
        OLED_ShowWord(page->coorX[i], page->coorY[i], page->txt + page->num2[i], page->num[i], 0);
        //OLED_ShowWord(page->coorX[i], 6, page->num2[i] + page->txt, 2, 0);     //page->coorY[i]  page->num[i]
    }
}








