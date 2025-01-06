#include "pch.h"
#include "TutorialMgr.h"
#include "QuestMsgBox.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "ProgressBar.h"
#include "SoundMgr.h"

CTutorialMgr* CTutorialMgr::m_pInstance = nullptr;

CTutorialMgr::CTutorialMgr()
	:m_eCurQuest(QUEST_END), m_iProgress(0), m_iProgressMax(0),
	m_pMsgBox(nullptr), m_pProgressBar(nullptr)
{
}

CTutorialMgr::~CTutorialMgr()
{
}

void CTutorialMgr::Initialize()
{
	//게임속도 변경하라는 튜토리얼 퀘스트 시작
	Create_MsgBox();
	Create_ProgressBar();

	m_eCurQuest = QUEST_START;
	Start_NextQuest();
}

//void CTutorialMgr::Check_Quest()
//{
//
//}

void CTutorialMgr::IncreaseQuestProgress()
{
	++m_iProgress;
	static_cast<CProgressBar*>(m_pProgressBar)->Set_Value((float)m_iProgress, (float)m_iProgressMax);

	if (m_iProgress >= m_iProgressMax)
	{
		CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"LessonActivated.wav", SOUND_UI, 1.f);

		m_iProgress = 0;
		static_cast<CProgressBar*>(m_pProgressBar)->Set_Value((float)m_iProgress, (float)m_iProgressMax);

		//다음 퀘스트 시작 //다음 퀘스트 시작전에 몇초 기다렸다가 하는건 어떠한가
		Start_NextQuest();

		//퀘스트 다 했으면 UI 삭제
		if (m_eCurQuest == QUEST_END)
		{
			m_pMsgBox->Set_Destroyed();
			m_pProgressBar->Set_Destroyed();
		}

	}
}

//void CTutorialMgr::Change_Quest(QUEST _eQuest)
//{
//	if (m_eCurQuest != _eQuest)
//	{
//		m_eCurQuest = _eQuest;
//		//메시지 박스의 렌더를 변경
//	}
//}

void CTutorialMgr::Create_MsgBox()
{
	m_pMsgBox = CAbstractFactory<CQuestMsgBox>::Create();
	m_pMsgBox->Set_Pos(WINCX - m_pMsgBox->Get_Info().fCX *0.5f, m_pMsgBox->Get_Info().fCY * 0.5f);
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pMsgBox);
}

void CTutorialMgr::Create_ProgressBar()
{
	m_pProgressBar = CAbstractFactory<CProgressBar>::Create();
	m_pProgressBar->Set_Size(200.f, 50.f);
	m_pProgressBar->Set_Pos(m_pMsgBox->Get_Rect()->left + m_pProgressBar->Get_Info().fCX * 0.5f + 25.f,
		m_pMsgBox->Get_Rect()->bottom - m_pProgressBar->Get_Info().fCY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_UI, m_pProgressBar);
}

void CTutorialMgr::Start_NextQuest()
{
	m_eCurQuest = (QUEST)((int)m_eCurQuest + 1);

	switch (m_eCurQuest)//퀘스트에 따라 퀘스트 수행 횟수 변경
	{
	case CTutorialMgr::QUEST_GAMESPEED:
		m_iProgressMax = 5;
		break;
	case CTutorialMgr::QUEST_DECONSTRUCT:
		m_iProgressMax = 5;
		break;
	case CTutorialMgr::QUEST_LOGGING:
		m_iProgressMax = 5;
		break;
	case CTutorialMgr::QUEST_CONSTRUCT_WALL:
		m_iProgressMax = 5;
		break;
	case CTutorialMgr::QUEST_CONSTRUCT_CAMPFIRE:
		m_iProgressMax = 1;
		break;
	case CTutorialMgr::QUEST_DRAFTE:
		m_iProgressMax = 5;
		break;
	case CTutorialMgr::QUEST_BATTLE:
		m_iProgressMax = 1;
		break;	
	case CTutorialMgr::QUEST_CONSTRUCT_SHIP:
		m_iProgressMax = 1;
		break;	
	case CTutorialMgr::QUEST_ESCAPE:
		m_iProgressMax = 1;
		break;
	case CTutorialMgr::QUEST_END:
		break;
	default:
		break;
	}
}
