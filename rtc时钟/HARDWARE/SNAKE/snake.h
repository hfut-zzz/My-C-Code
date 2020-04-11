#ifndef _SNAKE_H
#define _SNAKE_H

#include "sys.h"

#define OK 1    //����״̬
#define ERRO 0  //�Ƿ�״̬����Ϸ����

#define Move_Up     1     // ���ƶ�����
#define Move_Down   2
#define Move_Left   3
#define Move_Right  4


#define MAP_Witdh   32
#define MAP_Longth  64

typedef struct Coordinate
{
    u8 x;
    u8 y;
}Coordinate;

typedef struct SnakeNode       //�߽ڵ�
{
    Coordinate Coor;           //����
    struct SnakeNode *next;    //ָ����һ�ڵ�
}SnakeNode, * SnakePtr;

typedef struct Snake
{
    SnakePtr front;             // ��ͷ
    SnakePtr rear;              // ��β
    u8 length;                  //����
}Snake;

typedef struct Food
{
    Coordinate Coor;
}Food;

u8 SnakeMove(Snake * snake,Food* food, u8 dir); 

u8 PlaySnake(void);                 //���㺯����̰������Ϸ��ѭ��
void DrawWall(void); 
void DestroySnake(Snake * snake); 
void lost(Snake* snake, Food* food);
void CreatFood(Snake* snake, Food* food);
void Snake_Init(Snake * snake, Food* food);
void InsertFront(Snake * snake, Coordinate * coor);

#endif
