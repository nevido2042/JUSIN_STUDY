#include "pch.h"
#include "TutorialMgr.h"
#include "QuestMsgBox.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "ProgressBar.h"

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
	m_iProgressMax = 5;
	//게임속도 변경하라는 튜토리얼 퀘스트 시작
	Create_MsgBox();
	Create_ProgressBar();
	Change_Quest(QUEST_GAMESPEED);
}

void CTutorialMgr::Check_Quest()
{

}

void CTutorialMgr::IncreaseQuestProgress()
{
	++m_iProgress;
	static_cast<CProgressBar*>(m_pProgressBar)->Set_Value((float)m_iProgress, (float)m_iProgressMax);

	if (m_iProgress >= m_iProgressMax)
	{
		m_iProgress = 0;
		static_cast<CProgressBar*>(m_pProgressBar)->Set_Value((float)m_iProgress, (float)m_iProgressMax);

		m_eCurQuest = (QUEST)((int)m_eCurQuest + 1);
		//퀘스트 다 했으면 UI 삭제
		if (m_eCurQuest == QUEST_END)
		{
			m_pMsgBox->Set_Destroyed();
			m_pProgressBar->Set_Destroyed();
		}

	}
}

void CTutorialMgr::Change_Quest(QUEST _eQuest)
{
	if (m_eCurQuest != _eQuest)
	{
		m_eCurQuest = _eQuest;
		//메시지 박스의 렌더를 변경
	}
}

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
