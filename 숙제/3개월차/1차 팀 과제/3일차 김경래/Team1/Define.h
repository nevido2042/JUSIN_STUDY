#pragma once
#define WINCX		800
#define WINCY		600

#define GAME_WINCX      600
#define GAME_WINCY      550

#define GAME_WIN_LEFT      (WINCX - GAME_WINCX) * 0.5f
#define GAME_WIN_TOP      (WINCY - GAME_WINCY) * 0.5f
#define GAME_WIN_RIGHT      (WINCX - GAME_WINCX) * 0.5f + GAME_WINCX
#define GAME_WIN_BOTTOM      (WINCY - GAME_WINCY) * 0.5f + GAME_WINCY

#define	PURE		= 0
#define PI			3.141592f

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1
#define OBJ_CLEAR			2
#define OBJ_FINISH			3

extern HWND		g_hWnd;

enum SCENE { START, STAGE_ONE, END, NONE };
enum DIR { LEFT, TOP, RIGHT, BOTTOM, NODIR };
enum OBJID { OBJ_PLAYER, OBJ_PLAYER_BULLET, OBJ_MONSTER_BULLET, OBJ_MONSTER, OBJ_SATELLITE, OBJ_ITEM, OBJ_END };
enum BULLETTYPE {BULLET_ONE , BULLET_TWO, BULLET_THREE, BULLET_SCREW};
enum ITEMTYPE {ITEM_PORTION, ITEM_DAMAGEUP, ITEM_SATELLITE, ITEM_SPEEDUP, ITEM_BULLETTWO, ITEM_BULLETSCREW, ITEM_END};
enum BULLETMAKER {BM_PLAYER, BM_MONSTER, BM_END};

typedef struct tagInfo
{
	float		fX, fY;
	float		fCX, fCY;	// 가로, 세로 길이

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