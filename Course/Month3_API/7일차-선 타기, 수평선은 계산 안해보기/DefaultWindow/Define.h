#pragma once

#define WINCX		800
#define WINCY		600

#define	PURE		= 0
#define PI			3.141592f

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1	

extern HWND		g_hWnd;

enum DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_LU, DIR_RU, DIR_END };

enum OBJID { OBJ_PLAYER, OBJ_BULLET, OBJ_MONSTER, OBJ_MOUSE, OBJ_SHIELD, OBJ_BUTTON, OBJ_END };

typedef struct tagInfo
{
	float		fX, fY;
	float		fCX, fCY;	// ����, ���� ����

}INFO;

typedef struct tagLinePoint
{
	float		fX, fY;

	tagLinePoint()	{ZeroMemory(this, sizeof(tagLinePoint));}
		tagLinePoint(float _fX, float _fY)
		: fX(_fX), fY(_fY)
	{	}

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT	tLPoint;
	LINEPOINT	tRPoint;

	tagLine() { 	ZeroMemory(this, sizeof(tagLine));	}
	tagLine(LINEPOINT& _tLPoint, LINEPOINT& _tRPoint)
		: tLPoint(_tLPoint), tRPoint(_tRPoint) {	}

}LINE;


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}