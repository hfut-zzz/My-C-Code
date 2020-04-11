#include "snake.h"
#include "timer.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include <stdio.h>
#include <stdlib.h>


void CreatFood(Snake* snake, Food* food)
{
    u8 x, y;            //��Ҫ���Ǹı�����
    u8 ok = 0;
    u8 flag = 1;
    SnakeNode* p;
    while(!ok)
    {
        flag = 1;
        p = snake->front;
        x = GetRandom(1, MAP_Longth -2);
        y = GetRandom(1, MAP_Witdh - 2);

        while(p)
        {
            if((x == p->Coor.x) && (y == p->Coor.y))
            {
                 ok = 0;
                 flag = 0;
                 break;
            }
            p = p->next;                
        }
        if(flag)
            ok = 1;
    }
    p = NULL;
    food->Coor.x = x;
    food->Coor.y = y;

    XY_On(2 * x, 2 * y);
}

void DrawWall(void)
{
    u8 i, j;
    for(i = 0; i < MAP_Longth; i++)
        for(j = 0; j < MAP_Witdh; j++)
             if(i == 0 || i == MAP_Longth - 1 || j == 0 || j == MAP_Witdh - 1)
                 XY_On(2 * i, 2 * j);
}

void Snake_Init(Snake * snake, Food* food)    //��ʼ��   �����������ڵ����
{   
   
    SnakeNode* PNew;
   
    PNew = (SnakeNode *) malloc (sizeof(SnakeNode));    //����һ���ڴ棬�洢��ͷ�ڵ�
      
    snake->front = PNew;
    snake->rear = PNew;
     
    snake->length = 1;
    
    PNew->next = NULL;
    PNew->Coor.x = 31;
    PNew->Coor.y = 15;  //��ͷ����
    
    XY_On((PNew->Coor.x) * 2, (PNew->Coor.y) * 2); //��oled����ʾ
    
    PNew = (SnakeNode *) malloc (sizeof(SnakeNode)); //��β
    PNew->next = NULL;
    PNew->Coor.x = 30;
    PNew->Coor.y = 15;
    
    XY_On((PNew->Coor.x) * 2, (PNew->Coor.y) * 2);
    
    snake->rear = PNew;
    snake->front->next = PNew;
    snake->length = 2;
    
    DrawWall();
    CreatFood(snake, food);
    
}

//����һ���ߣ��ͷ��ڴ棩
void DestroySnake(Snake * snake)
{
    SnakeNode * p = snake->front;
    SnakeNode * q;
    
    while(p)
    {
        q = p->next;
        XY_Off((p->Coor.x) * 2, (p->Coor.y) * 2);
        free(p);
        p = q;
    }
    snake->rear =  NULL;
    snake->front = NULL;
    snake->length = 0;
}

void InsertFront(Snake * snake, Coordinate * coor)
{
    SnakeNode * pNew;

    pNew = (SnakeNode*) malloc (sizeof(SnakeNode));
    
    pNew->next = NULL;
    pNew->Coor.x = coor->x;
    pNew->Coor.y = coor->y;
    
    pNew->next = snake->front;
    snake->front = pNew;
    
    XY_On(coor->x * 2, coor->y * 2);
      
}

void DestroyRear(Snake* snake)
{
    Coordinate coor;
    SnakeNode* p = snake->front; //�Ȱ�Pָ����ͷ
    while(p->next != snake->rear)//��pָ��Ľڵ�����βʱֹͣ�����ҵ���βǰһ���ڵ�
    {
         p = p->next;
    }
 
    coor.x = snake->rear->Coor.x;  //  ������β�����꣬�������ʹ��
    coor.y = snake->rear->Coor.y;
    
    free(p->next);
    snake->rear = p;                //�����޸���β�ڵ�
    snake->rear->next = NULL;
    
    XY_Off(coor.x * 2, coor.y * 2);
}

u8 SnakeMove(Snake * snake,Food* food, u8 dir)   //�߰���ָ�������ƶ�һ�� ����ֵ�����ж����Ƿ��ڷǷ�״̬
{
    Coordinate coor;
    SnakeNode* pNew = snake->front;    //�����ж����Ƿ�Ե��Լ�
    
    if(dir == Move_Up)
    {
        coor.x = snake->front->Coor.x; 
        coor.y = snake->front->Coor.y - 1;
    }
    else if(dir == Move_Down)
    {
        coor.x = snake->front->Coor.x; 
        coor.y = snake->front->Coor.y + 1;
    }    
    else if(dir == Move_Left)
    {
        coor.x = snake->front->Coor.x - 1; 
        coor.y = snake->front->Coor.y;
    }     
    else if(dir == Move_Right)
    {
        coor.x = snake->front->Coor.x + 1; 
        coor.y = snake->front->Coor.y;
    }
//    else
//        return ERRO;
     
    if((food->Coor.x == coor.x) && (food->Coor.y == coor.y)) //�Ե�ʳ��
    {
        LED1 = LED_ON;
        InsertFront(snake, &coor);
        CreatFood(snake, food); //��������һ��ʳ��
        (snake->length)++;
        return OK;
    }
        
    while(pNew)
    {
         if(((pNew->Coor.x) == coor.x) && ((pNew->Coor.y) ==  coor.y))
         {
             return ERRO;        //�����ߵ����нڵ㣬����Ե����Լ��ͽ�����Ϸ             
         }
         pNew = pNew->next;
    }
    if((coor.x <= 0) || (coor.x >= MAP_Longth - 1) || (coor.y <= 0) || (coor.y >= MAP_Witdh - 1))
        return ERRO;             //��ײǽ�ˣ���Χ��һȦ��ǽ��ռһ���㣩
    
    InsertFront(snake, &coor);
    DestroyRear(snake);
    return OK;
}

void lost(Snake* snake, Food* food)
{
    u8 score = 0;
    u8 temp;    
    score = 10 * (snake->length - 2);
    temp = score / 100;

    DestroySnake(snake);
    XY_Off(2 * food->Coor.x, 2 * food->Coor.y); //����ʳ��

    OLED_ShowChinese(32, 2, 0, 0); //��Ϸ����
    OLED_ShowChinese(48, 2, 1, 0);
    OLED_ShowChinese(64, 2, 2, 0);
    OLED_ShowChinese(80, 2, 3, 0);

    OLED_ShowChinese(32, 4, 4, 0); //�÷�
    OLED_ShowChinese(48, 4, 5, 0);
    OLED_ShowNum(65, 4, score, 2 + temp, 16, 0, 0);     
}


u8 PlaySnake(void)
{
    Snake snake;
    Food food;
    u8 dir = 4;
    u8 old = 4;
    u8 temp = 1;        //ѡ��temp
    u8 choice = 0;      //ѡ�� 1��ʼ  2����  3�˳�   û��ѡ���Ĭ��0
    u8 txt[15] = {6, 7, 8, 9, 0, 1, 10, 11, 5, 18, 16, 17, 19, 10, 20};    //̰����С��Ϸ ��ʼ ���� �˳� ������
//    u8 txt_start[2] = {10, 11};
//    u8 txt_scort[2] = {5, 18};
//    u8 txt_exit[2] = {16, 17};
    u8 key;
    OLED_ShowWord(16, 0, txt, 6, 0);   
    OLED_ShowWord(48, 2, txt + 6, 2, 1);
    OLED_ShowWord(48, 4, txt + 8, 2, 0);
    OLED_ShowWord(48, 6, txt + 10, 2, 0);
    while(1)          
    {
         key = KEY_Scan(0);  
         if(key == KEY0_PRES)
         {
             OLED_ShowWord(48, 2 * temp, txt + 4 + 2 * temp, 2, 0);
             temp--;
             if(temp <= 0)
                 temp = 3;
             OLED_ShowWord(48, 2 * temp, txt + 4 + 2 * temp, 2, 1);
         }
         if(key == KEY1_PRES)
         {
              choice = temp;
         }
         if(key == WKUP_PRES)
         {
             OLED_ShowWord(48, 2 * temp, txt + 4 + 2 * temp, 2, 0);
             temp++;
             if(temp > 3)
                 temp = 1;
             OLED_ShowWord(48, 2 * temp, txt + 4 + 2 * temp, 2, 1);
         }
         
         if(choice == 3)      //�˳�
         {
              OLED_Clear();
              return 0;
         }           
         if(choice == 2)      //��߷�
         {
             OLED_Clear();
             choice = 0;
             temp = 1;             
             while(1)
             {
                
                 key = KEY_Scan(0); 
                 OLED_ShowWord(40, 3, txt + 12, 3, 0);
                 //ShowScore()    Ŀǰ�������ǰ���ʷ��߷ּ�¼��eeprom�У�ÿ����Ϸ�������������򣬸�����߷�
                 if(key)
                 {
                     OLED_Clear();
                     OLED_ShowWord(16, 0, txt, 6, 0);   
                     OLED_ShowWord(48, 2, txt + 6, 2, 1);
                     OLED_ShowWord(48, 4, txt + 8, 2, 0);
                     OLED_ShowWord(48, 6, txt + 10, 2, 0);
                     break;
                 }
             }
         }             

         if(choice == 1)     //��Ϸ��ʼ
         {
            OLED_Clear();
            Snake_Init(&snake, &food);
            while(1)
            { 
                old = dir;
                dir = GetDirection();
                
                    if(dir == 0);
                    else if(old == 0);             
                    else if((old + dir == 3) || (dir + old == 7))
                        dir = old;    //���򲻱䣬��ֹ��ͷ������������
                
                while(!dir)            //��ͣ
                     dir = GetDirection();
                if(SnakeMove(&snake, &food, dir)) ;
                else
                {
                    lost(&snake, &food); 
                    key = 0;
                    while(dir && !key)            //��ͣ
                    {
                        //key = KEY_Scan(0);       //���������
                         dir = GetDirection();                    
                    }
                    OLED_Clear();
                    PlaySnake();        //�˳�����һ������
                    OLED_Clear();
                    return 0;           //ѡ���˳�
                } 
                delay_ms(200);
                delay_ms(200);
                delay_ms(200);
                delay_ms(200);
                delay_ms(200);
                delay_ms(200);
                delay_ms(200);
                delay_ms(200);
            }            
         }                   
    }
}















