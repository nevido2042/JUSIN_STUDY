#pragma once
#include "pch.h"

enum TILE_TYPE { NONE = 0, ROAD, WALL, START, END, NODE, PATH, CHECK };

enum DIRECTION { OO, UU, RU, RR, RD, DD, LD, LL, LU }; //12시 ~ 1시 방향 순

#define SLEEP_TIME 10

template<typename T>
void Safe_Delete(T& _p)
{
    //T& p = const_cast<T&>(_p);

    if (_p)
    {
        delete _p;
        _p = nullptr;
    }
}

typedef struct tagPosition
{
public:
    int iX;
    int iY;
    tagPosition() :iX(0), iY(0) {};
    tagPosition(int _iX, int _iY) 
        :iX(_iX), iY(_iY)
    {

    }

    bool operator==(const tagPosition& _Pos)
    {
        if (iX == _Pos.iX && iY == _Pos.iY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator==(const tagPosition& _Pos) const
    {
        if (iX == _Pos.iX && iY == _Pos.iY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator!=(const tagPosition& _Pos)
    {
        if (iX != _Pos.iX && iY != _Pos.iY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator!=(const tagPosition& _Pos) const
    {
        if (iX != _Pos.iX && iY != _Pos.iY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}POS;

//색상
enum COLOR
{
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