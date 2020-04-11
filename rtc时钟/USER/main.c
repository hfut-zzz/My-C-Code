#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 		 	 
#include "oled.h"	
#include "key.h" 	 
#include "string.h"
#include "snake.h"
#include "timer.h"
#include "rtc.h"
#include "page.h"
#include "remote.h"


u8 key2;    //���ڳ������ 
Page pageMain;
extern u16 ADCConvertedValue[10][2];
extern u16* ps2_val;

int main(void)
{	 
    u8 count;   //��ⳤ��ʹ�õı���
    //u8 txt_time[2] = {14, 15};
    u8 menu[2 ] = {0, 1};    //��Ϸ ���� ����
    u8 menu_choce = 0;     //�˵����棬ָ���ӹ��ܣ�0:��Ϸ
    u8 key;     //���ڵ㰴���
    
    
    
    RTC_Init();
 	delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
    TIM3_Int_Init(4999, 71);    //5ms
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������ 
    OLED_Init();
    OLED_Clear();
    
    Page_main(&pageMain);
    ShowPage(&pageMain);
    
    AdcInit();
	DMAInit();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//��ʼ�ɼ�    
       
    while(1)
    {
        key = KEY_Scan(1);
        //PlaySnake();       
        if(key == KEY1_PRES)   //����˵�ҳ��
        {
            OLED_Clear();
            key = 0;
            while(1)
            {
                key = KEY_Scan(0);
                key2 = KEY_Scan(1); 
                OLED_ShowWord(48, 6, menu, 2, 0); //��ֻ��ʾ���֣�ͼ�񲿷��Ȳ���
                
                if(key2 == KEY0_PRES)
                     count++;
                else 
                    count = 0;
                if(count >= 20)
                {
                    OLED_Clear();
                    ShowPage(&pageMain);
                    break;                
                }
                if(key == KEY0_PRES)
                {
                     menu_choce++;
                  
                    //��ʾ����ӹ���
                }
                if(key == WKUP_PRES)
                {
                     menu_choce--;
                    //��ʾ����ӹ���
                }
                if(key == KEY1_PRES)
                    PlaySnake();    //������Ϸ
                

                delay_ms(20);
            }            
        }

        
        OLED_ShowTime(calendar.x, calendar.y, calendar.hour, calendar.min, calendar.sec); 
        LED1 =  ~LED1;
        delay_ms(200);
    }
}

