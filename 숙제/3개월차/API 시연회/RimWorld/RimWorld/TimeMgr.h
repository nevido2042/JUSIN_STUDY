#pragma once
#include "TutorialMgr.h"

class CTimeMgr
{
private:
	CTimeMgr();
	~CTimeMgr();

public:
	int		Get_CurrentFrame() { return m_iCurFrame; }
	float	Get_GameSpeed() { return m_fGameSpeed; }
	void	Set_GameSpeed(float _fGameSpeed) 
	{
		CTutorialMgr::QUEST eCurQuest = CTutorialMgr::Get_Instance()->Get_CurQuest();
		if (eCurQuest == CTutorialMgr::QUEST::QUEST_GAMESPEED)
		{
			CTutorialMgr::Get_Instance()->IncreaseQuestProgress();
		}

		m_fGameSpeed = _fGameSpeed; 
	}
	float	Get_ElapsedTime() { return m_fElapsedTime; }

public:
	void Initialize();
	void Update();
	void Render(HDC hDC);

public:
	static CTimeMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTimeMgr;

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
	static CTimeMgr* m_pInstance;

private:
	int			m_iCurFrame;
	float		m_fGameSpeed;
	float		m_fElapsedTime;
};

