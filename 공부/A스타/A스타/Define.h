#pragma once
#include "pch.h"

enum TILE_TYPE { NONE = 0, ROAD, WALL, START, END, NODE };

typedef struct tagPosition
{
    int iX;
    int iY;
    tagPosition() :iX(0), iY(0) {};
    tagPosition(int _iX, int _iY) 
        :iX(_iX), iY(_iY)
    {

    }
}POS;

//����
enum {
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DAKR_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

// �ܼ� �ؽ�Ʈ ���� �������ִ� �Լ�
void Set_Color(unsigned short text);