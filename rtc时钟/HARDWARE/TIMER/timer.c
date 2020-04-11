#include "timer.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "page.h"
#include "remote.h"
#include "oled.h"

u8 key = 0;
extern u8 key2;
u8 flag;        //���ڳ������
u8 count2 = 0;  //���ڳ������
u8 count = 0;
u8 direc = 4;      //���µķ�������
u8 old = 4;        //��һ�εķ�������
u16* ps2_val;
static u8 ps2_flag = 1;

vu8 frame_cnt; 
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		   	    
	//OSIntEnter();        
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)//����ж�
	{
        //�޸�ʱ��
        //key = KEY_Scan(0);
        key2 = KEY_Scan(1);
        ps2_val = PS2_Scan();
        if(key2 == WKUP_PRES)
            flag = 1;
        else 
            flag = 0;
        if(flag == 1 && key2 == WKUP_PRES)
            count2++;
        else 
            count2 = 0;
        if(count2 >= 60)
        {
            OLED_Clear();
            SetTime();            
        }   
        
        if(ps2_flag == 0)
            key = 0;
        //�ж�ps2ң�еĶ���
        if(ps2_flag && ps2_val[2] == 0) 
        {
            key = KEY1_PRES;
            direc = 0;    
        } 
        if(ps2_flag && (ps2_val[0] > 200 || ps2_val[0] < 130 || ps2_val[1] > 200 || ps2_val[1] < 130))
        {
            ps2_flag = 0;
            if(ps2_val[0] > ps2_val[1])  //y�����ݴ���x��  y������ �� x������
            {
                if(ps2_val[0] > 200)   //����
                {
                    key = WKUP_PRES;
                     direc = 2;
                }
                else                   //����
                    direc = 4;
            }
            if(ps2_val[0] < ps2_val[1])
            {
                if(ps2_val[0] < 130)   //����
                {
                    key = KEY0_PRES;
                    direc = 1;
                }
                else                   //����
                    direc = 3;    
            }
        }
        if(ps2_val[0] < 170 && ps2_val[0] > 160 && ps2_val[1] > 160 && ps2_val[1] < 170)
        {
             key = 0 ;
             ps2_flag = 1;
        }
            

        //Ϊfood�����ṩ�����
        count++;
        if(count >= 250)
            count = 0;

	}				   
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ 	 			    
	//OSIntExit();  											 
}

u8 GetDirection(void)
{
//    if(direc == 0);
//    else if(old == 0);             
//    else if((old + direc == 3) || (direc + old == 7))
//        direc = old;    //���򲻱䣬��ֹ��ͷ������������
    return direc;    //��������direc�����ˣ����𲻵��жϷ�������ԵĹ�����
}

u8 GetRandom(u8 min, u8 max)
{
    u8 result;
    result = (count % (max - min + 1)) + min;
    return result;
}

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
	
}
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3											  
}  	 



