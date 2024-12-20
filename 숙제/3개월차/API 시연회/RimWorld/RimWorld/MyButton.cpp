#include "pch.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{
	m_eRenderID = RENDER_UI;
}

int CMyButton::Update()
{
	if (m_bDestroyed)
		return OBJ_DESTROYED;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	if (!m_bActivate)
	{
		return;
	}

	if (Is_MouseHovered())
	{
		m_iDrawID = 1;
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			if (!lstrcmp(L"Start", m_pImgKey))
			{
				CSceneMgr::Get_Instance()->Set_Scene(SC_COLONY);

			}
			else if (!lstrcmp(L"Exit", m_pImgKey))
			{
				DestroyWindow(g_hWnd);
			}
			else if (!lstrcmp(L"ButtonSubtleAtlas_Architect", m_pImgKey))
			{
				//구상 버튼을 누르면
				//명령, 구조물 버튼이 활성화 된다.(자식 오브젝트)

				//이미 자식들이 켜져있었으면 손자들까지 모두다 끄게 만들기
				for (CObj* pChild : *Get_ChildList())
				{
					if (pChild->Get_Activate())
					{
						pChild->Set_Activate(false);

						for (CObj* pGrandchild : *pChild->Get_ChildList())
						{
							if (pGrandchild->Get_Activate())
							{
								pGrandchild->Set_Activate(false);
							}
						}

					}
					else
					{
						pChild->Set_Activate(true);
					}
					
				}
			}
			else if (!lstrcmp(L"ButtonSubtleAtlas_Command", m_pImgKey))
			{
				//명령 버튼을 누르면
				//해체 버튼이 활성화 된다.
				//구상의 구조물 버튼과 그 하위 버튼이 비활성화 된다.

				//부모의 하위버튼 중 자신 빼고 하위버튼의 자식들을 비활성화
				for (CObj* pParentChild : *Get_Parent()->Get_ChildList())
				{
					if (pParentChild->Get_Activate()&& pParentChild != this)
					{
						for (CObj* Nephew : *pParentChild->Get_ChildList())
						{
							if (Nephew->Get_Activate())
							{
								Nephew->Set_Activate(false);
							}
						}
					}
				}
				
				for (CObj* pObj : *Get_ChildList())
				{
					if (pObj->Get_Activate())
					{
						pObj->Set_Activate(false);
					}
					else
					{
						pObj->Set_Activate(true);
					}
				}
			}
			else if (!lstrcmp(L"ButtonSubtleAtlas_Structure", m_pImgKey))
			{
				//구조물 버튼을 누르면
				//벽 건설 버튼이 활성화 된다.
				//구상의 명령 버튼과 그 하위 버튼이 비활성화 된다.
				//부모의 하위버튼 중 자신 빼고 하위버튼의 자식들을 비활성화
				for (CObj* pParentChild : *Get_Parent()->Get_ChildList())
				{
					if (pParentChild->Get_Activate() && pParentChild != this)
					{
						for (CObj* Nephew : *pParentChild->Get_ChildList())
						{
							if (Nephew->Get_Activate())
							{
								Nephew->Set_Activate(false);
							}
						}
					}
				}

				for (CObj* pObj : *Get_ChildList())
				{
					if (pObj->Get_Activate())
					{
						pObj->Set_Activate(false);
					}
					else
					{
						pObj->Set_Activate(true);
					}
				}
			}
			else if (!lstrcmp(L"Deconstruct", m_pImgKey))
			{
				CColonyMgr::Get_Instance()->Change_Mode(CColonyMgr::MODE_DECONSTRUCT);
			}
			else if (!lstrcmp(L"Construct", m_pImgKey))
			{
				CColonyMgr::Get_Instance()->Change_Mode(CColonyMgr::MODE_CONSTRUCT);
			}
		}
	}
	else
	{
		m_iDrawID = 0;
	}
}

void CMyButton::Render(HDC hDC)
{
	if (!m_bActivate)
	{
		return;
	}

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	
		BitBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_iDrawID * (int)m_tInfo.fCX,
		0,
		SRCCOPY);
}

void CMyButton::Release()
{
}
