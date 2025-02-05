#include "pch.h"
#include "AbstractFactory.h"
#include "BmpMgr.h"
#include "ColonyMgr.h"
#include "ObjMgr.h"

#include "SoundMgr.h"
#include "TileMgr.h"
#include "Tree.h"
#include "TutorialMgr.h"

CTree::CTree()
{
}

CTree::~CTree()
{
    Release();
}

void CTree::Initialize()
{
	CBreakable::Initialize();

	Set_ObjID(OBJ_TREE);

	Set_ImgKey(L"TreePoplarA");

    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 64.f;

    m_eRenderID = RENDER_GAMEOBJECT;
}

int CTree::Update()
{
	if (m_bBrokendown)
	{
		if (CTutorialMgr::Get_Instance()->Get_CurQuest() == CTutorialMgr::QUEST_LOGGING)
		{
			CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
		}

		CSoundMgr::Get_Instance()->StopSound(SOUND_TREE);
		CSoundMgr::Get_Instance()->PlaySound(L"Tree_Felled_1a.wav", SOUND_TREE, .5f);
		//Set_Destroyed();

		//해당 타일위에 있는 Obj를  nullptr로 만든다.
		CTileMgr::Get_Instance()->Set_TileObj(m_tInfo.fX, m_tInfo.fY, nullptr);

		//원목을 자기 위치에 생성
		CObj* pObj = CAbstractFactory<CWoodLog>::Create(m_tInfo.fX, m_tInfo.fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_ITEM, pObj);

		//생성되었을 때 타일에게 아이템이 있음을 알림
		//CTileMgr::Get_Instance()->Set_TileObj(m_tInfo.fX, m_tInfo.fY, pObj);

		set<TASK>& LoggingSet = *CColonyMgr::Get_Instance()->Get_LoggingSet();
		for (auto Iter = LoggingSet.begin(); Iter != LoggingSet.end();)
		{
			if ((*Iter).pObj == this)
			{
				Iter = LoggingSet.erase(Iter);
			}
			else
			{
				++Iter;
			}
		}
	}

	return CBreakable::Update();
}

void CTree::Late_Update()
{
}

void CTree::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	POINT tPoint = CCamera::Get_Instance()->WorldToScreen((float)(m_tRect.left - IMAGE_OFFSET_X), (float)(m_tRect.top - IMAGE_OFFSET_Y - 32));
	float fZoom = CCamera::Get_Instance()->Get_Zoom();
	GdiTransparentBlt(hDC,			// 복사 받을 DC
		tPoint.x, //m_tRect.left + iScrollX - IMAGE_OFFSET_X,	// 복사 받을 위치 좌표 X, Y	
		tPoint.y, //m_tRect.top + iScrollY - IMAGE_OFFSET_Y - 32,
		int(128 * fZoom),			// 복사 받을 이미지의 가로, 세로
		int(128 * fZoom),
		hMemDC,						// 복사할 이미지 DC	
		0,				// 비트맵 출력 시작 좌표(Left, top)
		0,
		128,			// 복사할 이미지의 가로, 세로
		128,
		RGB_PURPLE);		// 제거할 색상

	//작업 예약되었으면 해체 아이콘 출력
	if (m_bTaskReserved)
	{
		HDC		hCutPlantDC = CBmpMgr::Get_Instance()->Find_Image(L"CutPlant");

		float fZoom = CCamera::Get_Instance()->Get_Zoom();

		POINT tPoint = CCamera::Get_Instance()->WorldToScreen(
			(float)Get_Rect()->left,
			(float)Get_Rect()->top
		);

		GdiTransparentBlt(hDC,
			(int)tPoint.x,
			(int)tPoint.y,
			(int)(64 * fZoom),
			(int)(64 * fZoom),
			hCutPlantDC,
			0, 0,
			64,
			64,
			RGB_WHITE);
	}
}

void CTree::Release()
{
}
