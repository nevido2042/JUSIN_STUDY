#pragma once

#define WINCX				1280
#define WINCY				720

#define	PURE				= 0
#define PI					3.141592f

//������Ʈ ����
#define OBJ_NOEVENT			0
#define OBJ_DESTROYED		1

//Ű �Է�
#define VK_MAX				0xff

//�� �̹��� ������
#define HEAD_OFFSET			24
#define IMAGE_OFFSET_X		32
#define IMAGE_OFFSET_Y		32

//���� ����
#define RGB_PURPLE			RGB(163, 73, 164)
#define RGB_WHITE			RGB(255, 255, 255)

//Ÿ�� ����
#define	TILECX				64
#define	TILECY				64

#define TILEX				30
#define TILEY				20


//health�� offset
#define HELATHBAR_OFFSET    20

// Ÿ�� �ø� ������
#define CULLIG_OFFSET       2

//���Ӽӵ� ���
#define GAMESPEED           CTimeMgr::Get_Instance()->Get_GameSpeed()

extern HWND		    g_hWnd;
//extern ULONG_PTR g_GdiplusToken;

enum OBJID { OBJ_PROJECTILE, OBJ_MOUSE, OBJ_UI, OBJ_WALL, OBJ_TREE, OBJ_RIM, OBJ_ENEMY, OBJ_WEAPON, OBJ_END };

enum TILEOPT { OPT_REACHABLE, OPT_BLOCKED, OPT_END };

enum RENDERID { RENDER_BACKGROUND, RENDER_GAMEOBJECT, RENDER_WEAPON, RENDER_UI, RENDER_EFFECT, RENDER_END };

enum SCENEID { SC_LOGO, SC_MENU, SC_CUSTOMIZE, SC_COLONY, SC_END };

enum DIRECTION { OO, UU, RU, RR, RD, DD, LD, LL, LU, DIR_END }; //12�� ~ 1�� ���� ��

enum CHANNELID { SOUND_EXPLOSION, SOUND_EFFECT, SOUND_BGM, SOUND_END };


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

typedef struct tagFrame
{
	int			iFrameStart;
	int			iFrameEnd;
	int			iMotion;
	ULONGLONG	ullSpeed;
	ULONGLONG	ullTime;

}FRAME;


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

    tagPosition operator*(const int _int)
    {
        iX *= _int;
        iY *= _int;

        return tagPosition{ iX, iY };
    }

}POS;

template <typename Func>
void Bresenham(int iX1, int iY1, int iX2, int iY2, Func callback)
{
    int iDistX = abs(iX2 - iX1);
    int iDistY = abs(iY2 - iY1);
    int iDirX = (iX1 < iX2) ? 1 : -1;
    int iDirY = (iY1 < iY2) ? 1 : -1;
    int iErr = iDistX - iDistY;

    while (true)
    {
        // �ݹ� �Լ� ȣ��: ���� ��ǥ�� ����
        if (!callback(iX1, iY1))
        {
            break;  // �ݹ� �Լ����� false�� ��ȯ�ϸ� ��� ��� �ߴ�
        }

        // ��ǥ ������ ���������� ����
        if (iX1 == iX2 && iY1 == iY2)
        {
            break;
        }

        int iError = 2 * iErr;
        if (iError > -iDistY)
        {
            iErr -= iDistY;
            iX1 += iDirX;
        }
        if (iError < iDistX)
        {
            iErr += iDistX;
            iY1 += iDirY;
        }
    }
}
