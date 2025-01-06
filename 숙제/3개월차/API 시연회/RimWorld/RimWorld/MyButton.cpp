#include "pch.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "LaunchMsgBox.h"
#include "TutorialMgr.h"

CMyButton::CMyButton()
	:m_bOnHovered(false)
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
	return CObj::Update();
}

void CMyButton::Late_Update()
{
	if (!m_bActivate)
	{
		return;
	}

	if (Is_MouseHovered())
	{

		if (!m_bOnHovered)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
			CSoundMgr::Get_Instance()->PlaySound(L"MouseoverButtonCategory_2d.wav", SOUND_UI, .5f);
			m_bOnHovered = true;
		}

		m_iDrawID = 1;
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{

			CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
			CSoundMgr::Get_Instance()->PlaySound(L"Click.wav", SOUND_UI, .5f);
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
			else if (!lstrcmp(L"ButtonSubtleAtlas_Menu", m_pImgKey))
			{
				//메뉴 버튼을 누르면

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
			else if (!lstrcmp(L"GoToMain", m_pImgKey))
			{
				CSceneMgr::Get_Instance()->Set_Scene(SC_MENU);
			}
			else if (!lstrcmp(L"ShipBtn", m_pImgKey))
			{
				CColonyMgr::Get_Instance()->Change_Mode(CColonyMgr::MODE_SHIP);
			}
			else if (!lstrcmp(L"LaunchShipBtn", m_pImgKey))
			{
				//우주선이 있는가?

				//우주선에 탑승했는가?
				//list<CObj*> pRimList = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM];

				//페이드 아웃
				
				//페이드 아웃 끝나면 메시지 박스 띄우기

				CObj* pObj = CAbstractFactory<CLaunchMsgBox>::Create(WINCX * 0.5f, WINCY * 0.5f);
				CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);

				CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
				CSoundMgr::Get_Instance()->PlaySound(L"Ship_TakeOff_1.wav", SOUND_BGM, 1.f);

				if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_ESCAPE)
				{
					CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
				}
			}
			else if (!lstrcmp(L"LoggingBtn", m_pImgKey))
			{
				CColonyMgr::Get_Instance()->Change_Mode(CColonyMgr::MODE_LOGGING);
			}
			else if (!lstrcmp(L"CampfireBtn", m_pImgKey))
			{
				CColonyMgr::Get_Instance()->Change_Mode(CColonyMgr::MODE_CAMPFIRE);
			}
		}
	}
	else
	{
		m_iDrawID = 0;
		m_bOnHovered = false;
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
