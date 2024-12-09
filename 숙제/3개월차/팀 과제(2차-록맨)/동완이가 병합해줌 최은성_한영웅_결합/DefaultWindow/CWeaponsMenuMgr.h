#pragma once
#include "CObjMgr.h"

class CWeaponsMenuMgr
{
private:
	CWeaponsMenuMgr();
	~CWeaponsMenuMgr();
	CWeaponsMenuMgr(const CWeaponsMenuMgr& rhs) = delete;
	CWeaponsMenuMgr& operator =(const CWeaponsMenuMgr& rhs) = delete;

public:
	enum MENU_STATE { NONE, OPENING, ONSCREAN, CLOSING };

	bool Update();
	void Render(HDC hDC);
	void Release();
	static CWeaponsMenuMgr* Get_Instance();
	static void Destroy_Instance();
	const MENU_STATE& Get_MenuState() { return m_eState; }

	void Open_Menu(const int* _BulCount);

private:

	static CWeaponsMenuMgr* m_pInstance;

	MENU_STATE m_eState;
	ULONGLONG m_ullAnimationTIme;
	UINT m_iAnimationFrame;
	UINT m_iAnimationX;
	UINT m_iAnimationY;
	UINT m_iCX;
	UINT m_iCY;
	const int* m_pBulCount;

	unsigned int m_iSelect;

	ULONGLONG	m_ullBlink;
	bool		m_bBlink;

	void Opening();
	void OnScrean();
	void Closing();
};