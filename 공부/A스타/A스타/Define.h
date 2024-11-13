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

//색상
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

// 콘솔 텍스트 색상 변경해주는 함수
void Set_Color(unsigned short text);