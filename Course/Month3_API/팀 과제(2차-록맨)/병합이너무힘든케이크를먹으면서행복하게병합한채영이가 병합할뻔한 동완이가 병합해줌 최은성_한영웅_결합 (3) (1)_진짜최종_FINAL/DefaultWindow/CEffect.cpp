#include "pch.h"
#include "CEffect.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CEffect::CEffect() :iCount(0)
{
#pragma region 초기화
	ZeroMemory(&m_tCutState, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom01, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom02, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom03, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom04, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom05, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom06, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom07, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom08, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom09, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom10, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom11, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom12, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom13, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom14, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom15, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom16, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom17, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom18, sizeof(IMAGEINFO));
	ZeroMemory(&m_tBoom19, sizeof(IMAGEINFO));
#pragma endregion

}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/boss_bomb_all.bmp", L"Boom");

	m_tBoom01 = { 19,126,34, 38 };
	m_tBoom02 = { 64, 124, 45, 44 };
	m_tBoom03 = { 115,121, 45, 45 };
	m_tBoom04 = { 169,116, 57, 56 };
	m_tBoom05 = { 229,116, 60, 56 };
	m_tBoom06 = { 295,113,63, 59 };
	m_tBoom07 = { 366,113, 65, 63 };
	m_tBoom08 = { 433,108,78,70 };
	m_tBoom09 = { 517,105,77,77 };
	m_tBoom10 = { 596, 106, 73,76 };
	m_tBoom11 = { 670,103,81,83 };
	m_tBoom12 = { 755,90,95, 104 };
	m_tBoom13 = { 41,222,104,98 };
	m_tBoom14 = { 147,216, 93, 173 };
	m_tBoom15 = { 243,219,100,104 };
	m_tBoom16 = { 347,216,114,110 };
	m_tBoom17 = { 468,218,108,108 };
	m_tBoom18 = { 584, 205,127, 123 };
	m_tBoom19 = { 719, 208,670,116 };
}

int CEffect::Update()
{
	iCount++;
	
	__super::Update_Rect();

	if (iCount < 5)
	{
		m_tCutState = m_tBoom01;
	}
	else if (iCount < 10)
	{
		m_tCutState = m_tBoom02;
	}
	else if (iCount < 15)
	{
		m_tCutState = m_tBoom03;
	}
	else if (iCount < 20)
	{
		m_tCutState = m_tBoom04;
	}
	else if (iCount < 25)
	{
		m_tCutState = m_tBoom05;
	}
	else if (iCount < 30)
	{
		m_tCutState = m_tBoom06;
	}
	else if (iCount < 35)
	{
		m_tCutState = m_tBoom07;
	}
	else if (iCount < 40)
	{
		m_tCutState = m_tBoom08;
	}
	else if (iCount < 45)
	{
		m_tCutState = m_tBoom09;
	}
	else if (iCount < 50)
	{
		m_tCutState = m_tBoom10;
	}
	else if (iCount < 55)
	{
		m_tCutState = m_tBoom11;
	}
	else if (iCount < 60)
	{
		m_tCutState = m_tBoom12;
	}
	else if (iCount < 65)
	{
		m_tCutState = m_tBoom13;
	}
	else if (iCount < 70)
	{
		m_tCutState = m_tBoom14;
	}
	else if (iCount < 75)
	{
		m_tCutState = m_tBoom15;
	}
	else if (iCount < 80)
	{
		m_tCutState = m_tBoom16;
	}
	else if (iCount < 85)
	{
		m_tCutState = m_tBoom17;
	}
	else if (iCount < 90)
	{
		m_tCutState = m_tBoom18;
	}
	else if (iCount < 95)
	{
		m_tCutState = m_tBoom19;
	}
	else
	{
		return OBJ_DEAD;
	}
}

void CEffect::Late_Update()
{

}

void CEffect::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Boom");

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,						// 복사 받을 DC
		m_tInfo.fX+ iScrollX,
		m_tInfo.fY+ iScrollY,								// 복사 받을 위치 좌표 X, Y			
		m_tCutState._iCX, m_tCutState._iCY,				// 복사 받을 이미지의 가로, 세로
		hMemDC,									// 복사할 이미지 DC	
		m_tCutState._iX, m_tCutState._iY,				// 비트맵 출력 시작 좌표(Left, top)
		m_tCutState._iCX, m_tCutState._iCY,				/// 복사할 이미지의 가로, 세로
		RGB(128, 0, 128));						// 출력 효과 설정(그대로 출력)

}

void CEffect::Release()
{
}

// 1: 19,126,57,164 = 34 38
// 2: 64,124,109,168
// 3: 115,121,160,166
// 4: 169,116,226,172
// 5: 229,116,289,172
// 
// 6 : 295,113,358,172
// 
// 
// 7 : 366,113,431,176
// 
// 
// 8 : 433,108,511,178
// 
// 
// 9 : 517,105,594,182
// 
// 10 : 596, 106, 669, 182
// 
// 11 : 670,103,752,186
// 
// 
// 12 : 755,90,850,194
// 
// 
// 13 : 41,222,145,320
// 
// 
// 14 : 147,216,240,320
// 
// 
// 15 : 243,219,343,323
// 
// 
// 16 : 347,216,461,326
// 
// 17 : 468,218,576,321
// 
// 18 : 584,205,711,328
// 19 : 719,208,849,324

// m_tBoom01 = {19,126,34, 38 };
// m_tBoom02 = {64, 124, 45, 44 };
// m_tBoom03 = {115,121, 45, 45};
// m_tBoom04 = {169,116, 57, 56};
// m_tBoom05 = {229,116, 60, 56};
// m_tBoom06 = {295,113,63, 59 };
// m_tBoom07 = {366,113, 65, 63};
// m_tBoom08 = {433,108,78,70};
// m_tBoom09 = {517,105,77,77 };
// m_tBoom10 = {596, 106, 73,76};
// m_tBoom11 = {670,103,81,83};
// m_tBoom12 = {755,90,95, 104};
// m_tBoom13 = {41,222,104,98 };
// m_tBoom14 = {147,216, 93, 173};
// m_tBoom15 = {243,219,100,104 };
// m_tBoom16 = {347,216,114,110 };
// m_tBoom17 = {468,218,108,108};
// m_tBoom18 = { 584,205,127, 123};
// m_tBoom19 = {719,208,670,116};