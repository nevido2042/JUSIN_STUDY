#include "pch.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CStage_Chae.h"
#include "CStage_Hong.h"
#include "CStage_Hero.h"
#include "CStage_Moo.h"
#include "CStage_Silver.h"
#include "CScrollMgr.h"

#include "CObjMgr.h"
#include "CKeyMgr.h"
#include "CWeaponsMenuMgr.h"

//SCENE_TYPE g_CurrentStage = SCENE_TYPE::STAGE_START; // 기본값 START

CSceneMgr* CSceneMgr::m_pInstance = nullptr; // 스태틱 변수는 CPP에서 초기화해주기, 

void CSceneMgr::Set_Scene(SCENE_TYPE eType)
{
	if (m_pCurScene) // 만약 스테이지에 값이 들어있다면
		m_pCurScene->Release(); // 그 스테이지를 해제

	m_pCurScene = m_arrScene[(UINT)eType]; // 그리고 새로운 스테이지를 현재 스테이지에 업데이트
	//g_CurrentStage = (int)eType; // 현재 스테이지 업데이트
	m_pCurScene->Initialize(); //  현재 스테이지 이니셜라이즈 초기화 
}

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	,m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	/// 씬 모두 삭제
	for (UINT i = 0; i < (UINT)SCENE_TYPE::SCENE_END; i++)
	{
		if (nullptr != m_arrScene[i])
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneMgr::Initialize() // Scene 생성
{
	m_arrScene[(UINT)SCENE_TYPE::STAGE_START] = new CScene_Start; // 스테이지 고르는 스타트 스테이즈
	///////////////////////////////////////////////////////////////////////////////////
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Chae] = new CStage_Chae;     // 채영 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Hero] = new CStage_Hero;     // 영웅 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Hong] = new CStage_Hong;     // 홍동완 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Silver] = new CStage_Silver; // 은성 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Moo] = new CStage_Moo;       // 무결 스테이지
	//////////////////////////////////////////////////////////////////////////////
	
	Set_Scene(SCENE_TYPE::STAGE_START);// 첫 Scene은 무조건 게임화면 띄우기
	//필요에 따라서 Tool 스테이지 만들예정
}


void CSceneMgr::Update()
{
	if (CWeaponsMenuMgr::Get_Instance()->Update())
		return;
	if (CScrollMgr::Get_Instance()->Update())
		return;
	m_pCurScene->Update(); // 현재 스테이지 업데이트
}
void CSceneMgr::Late_Update()
{
	if (CWeaponsMenuMgr::Get_Instance()->Get_MenuState() != 0)
		return;
	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;
	CObjMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pCurScene->Render(hDC); // 현재 스테이지 렌더
	if (CWeaponsMenuMgr::Get_Instance()->Get_MenuState() != 0)
		CWeaponsMenuMgr::Get_Instance()->Render(hDC);
}




