#pragma once
#include "Obj.h"

class CTutorialMgr
{
private:
	CTutorialMgr();
	~CTutorialMgr();
public:
	enum QUEST { QUEST_GAMESPEED, QUEST_DECONSTRUCT, QUEST_END };
public:
	void	Initialize();
	void	Check_Quest();//퀘스트 진행상황 확인
	QUEST	Get_CurQuest() { return m_eCurQuest; }//현재 진행 중인 퀘스트 확인
	void	IncreaseQuestProgress();//퀘스트 진척도 올리기
private:
	//void Start_Tutorial_GameSpeed();
	//void Start_Tutorial_Deconstruct();
	void	Change_Quest(QUEST _eQuest);
	void	Create_MsgBox();//퀘스트에 따른 메시지 박스 출력
	void	Create_ProgressBar();//프로그레스 바 생성

public:
	static CTutorialMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTutorialMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTutorialMgr* m_pInstance;
	
	QUEST	m_eCurQuest;
	int		m_iProgress;
	int		m_iProgressMax;

	CObj*	m_pMsgBox;
	CObj*	m_pProgressBar;
};

