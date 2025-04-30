#pragma once
#define WINCX		800
#define WINCY		600

#define GAME_WINCX      600
#define GAME_WINCY      600

#define GAME_WIN_LEFT      0.f
#define GAME_WIN_TOP       0.f
#define GAME_WIN_RIGHT      GAME_WINCX 
#define GAME_WIN_BOTTOM     GAME_WINCY

#define	PURE		= 0
#define PI			3.141592f

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1
#define OBJ_CLEAR			2
#define OBJ_FINISH			3

extern HWND		g_hWnd;

enum SCENE { START, STAGE_ONE, STAGE_TWO, STAGE_HERO,STAGE_KIM, STAGE_YU, STAGE_CY, END, NONE };
enum DIR { LEFT, TOP, RIGHT, BOTTOM, NODIR };
enum OBJID { OBJ_PLAYER, OBJ_BULLET_PLAYER, OBJ_BULLET_MONSTER , OBJ_MONSTER, OBJ_ITEM, OBJ_SHIELD, OBJ_END };
enum BULLETTYPE {BULLET_ONE, BULLET_TWO, BULLET_THREE, BULLET_SCREW_TWO, BULLET_SCREW_THREE};
enum BULLETMAKER {BM_PLAYER, BM_MONSTER, BM_END};

typedef struct tagInfo
{
	float		fX, fY;
	float		fCX, fCY;	// ����, ���� ����

}INFO;


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}