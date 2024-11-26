#pragma once

#define WINCX		800
#define WINCY		600

#define MINI_WINCX_LEFT		WINCX * 0.1f
#define MINI_WINCY_TOP		WINCY * 0.1f
#define MINI_WINCX_RIGHT		WINCX * 0.9f
#define MINI_WINCY_BOTTOM		WINCY * 0.9f

#define	PURE		= 0
#define PI			3.141592f

extern HWND		g_hWnd;

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