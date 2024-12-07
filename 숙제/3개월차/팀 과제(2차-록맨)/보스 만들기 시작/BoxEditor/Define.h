#pragma once

#define WINCX		800
#define WINCY		600

#define	PURE		= 0
#define PI			3.141592f

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1	

#define VK_MAX				0xff

extern HWND		g_hWnd;

enum DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_LU, DIR_RU, DIR_END };

enum DRAWPOINT { HEAD, TAIL, DRAWPOINT_END };
enum DRAWDIR { NO_DIR, HORIZONTAL, VERTICAL, DRAWDIR_END };


enum OBJID 
{ 
	OBJ_PLAYER,
	OBJ_BULLET,
	OBJ_MONSTER,
	OBJ_MOUSE,
	OBJ_SHIELD,
	OBJ_BUTTON,
	OBJ_BLOCK,
	OBJ_END 
};

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

typedef struct tagBox
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