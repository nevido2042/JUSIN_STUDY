#pragma once

#define WINCX		800
#define WINCY		600

#define	PURE		= 0
#define PI			3.141592f

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1	

#define VK_MAX				0xff // 영웅코드

//#define STAGE_HONG			1
//#define STAGE_SILVER		2
//#define STAGE_HERO			3
//#define STAGE_CHAE			4
//#define STAGE_MOO           5
//#define STAGE_FINISH		6

#define VK_MAX				0xff

#define G_FORCE                9.80665f
#define SQUARE(X)            (X)*(X)
#define RADIAN(Degree)        (Degree)*PI/180.f
#define DEGREE(Radian)        (Radian)*180.f/PI
// 은성코드

extern HWND		g_hWnd;
//extern int g_CurrentStage; // 현재 스테이지 어떤 스테이지인지 

enum BULLET_ID{ BUL_NORMAL, BUL_END }; // 총알 타입  // 은성 코드
enum DIRECTION { DIR_NONE,DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_LU, DIR_RU, DIR_END };

enum DRAWPOINT { HEAD, TAIL, DRAWPOINT_END }; // 채영 코드
enum DRAWDIR { NO_DIR, HORIZONTAL, VERTICAL, DRAWDIR_END };

enum OBJID 
{ 
	OBJ_PLAYER, 
	OBJ_BULLET,OBJ_BLOCK,
	OBJ_MONSTER,
	OBJ_BOSS,
	OBJ_BOSSBULLET ,
	OBJ_MOUSE, 
	OBJ_WALL,// 무결이 코드에 의해 추가
	OBJ_END 
};

enum class GROUP_TYPE 
{
	OBJ_PLAYER,
	OBJ_PLYAER_MISSILE,
	OBJ_MONSTER,
	OBJ_BOSS,
	OBJ_BOSS_MISSLE,
	OBJ_ITEM,
	OBJ_BLOCK,
	OBJ_BUTTON,
	OBJ_END
};

enum class SCENE_TYPE // 씬 매니저 
{
	STAGE_START,
	STAGE_Hong, // 동완 스테이지 1
	STAGE_Silver,//은성 스테이지 2
	STAGE_Hero, // 영웅 스테이지 3
	STAGE_Chae, // 채영 스테이지 4 
	STAGE_Moo,  // 무결 스테이지 5
	SCENE_END = 32, // 앤드
   
};




typedef struct tagInfo
{
	float		fX, fY;// 중심축
	float		fCX, fCY;// 가로, 세로 길이

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
 
typedef struct tagBoxPoint // 무결이 코드
{
	float		fX, fY;

	tagBoxPoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagBoxPoint(float _fX, float _fY)
		: fX(_fX), fY(_fY)
	{	}

}BOXPOINT;


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

struct tagFinder
{
public:
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& rObj)
	{
		return !lstrcmp(m_pString, rObj.first);
	}

private:
	const TCHAR* m_pString;
};

struct DeleteMap
{
public:
	template<typename T>
	void	operator()(T& Pair)
	{
		if (Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};

typedef struct tagBox // 영웅코드
{
	LINEPOINT	tLT;
	LINEPOINT	tRT;
	LINEPOINT	tLB;
	LINEPOINT	tRB;

	tagBox() { ZeroMemory(this, sizeof(tagLine)); }
	/*tagBox(LINEPOINT& _tLT, LINEPOINT& _tRT, LINEPOINT& _tLB, LINEPOINT& _tRB)
		: tLT(_tLT), tRT(_tRT), tLB(_tLB), tRB(_tRB) {	}*/
	tagBox(LINEPOINT& _tCenter, float _fSize)
		:
		tLT{ _tCenter.fX - _fSize * 0.5f, _tCenter.fY - _fSize * 0.5f },
		tRT{ _tCenter.fX + _fSize * 0.5f, _tCenter.fY - _fSize * 0.5f },
		tLB{ _tCenter.fX - _fSize * 0.5f, _tCenter.fY + _fSize * 0.5f },
		tRB{ _tCenter.fX + _fSize * 0.5f, _tCenter.fY + _fSize * 0.5f }
	{}

}BOX;