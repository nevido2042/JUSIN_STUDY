#include "pch.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CStage_Chae.h"
#include "CStage_Hong.h"
#include "CStage_Hero.h"
#include "CStage_Moo.h"
#include "CStage_Silver.h"

//SCENE_TYPE g_CurrentStage = SCENE_TYPE::STAGE_START; // 기본값 START

CSceneMgr* CSceneMgr::m_pInstance = nullptr; // 스태틱 변수는 CPP에서 초기화해주기, 

void CSceneMgr::Set_Scene(SCENE_TYPE eType)
{
	if (m_pCurScene)
		m_pCurScene->Release();

	m_pCurScene = m_arrScene[(UINT)eType];
	//g_CurrentStage = (int)eType; // 현재 스테이지 업데이트
	m_pCurScene->Initialize();
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
	m_arrScene[(UINT)SCENE_TYPE::STAGE_START] = new CScene_Start;

	m_arrScene[(UINT)SCENE_TYPE::STAGE_Chae] = new CStage_Chae;     // 채영 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Hero] = new CStage_Hero;     // 영웅 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Hong] = new CStage_Hong;     // 홍동완 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Silver] = new CStage_Silver; // 은성 스테이지
	m_arrScene[(UINT)SCENE_TYPE::STAGE_Moo] = new CStage_Moo;       // 무결 스테이지

	
	Set_Scene(SCENE_TYPE::STAGE_START);// 첫 Scene 설정
}


void CSceneMgr::Update()
{
	m_pCurScene->Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pCurScene->Render(hDC);
}




