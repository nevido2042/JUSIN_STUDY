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
	void	Check_Quest();//����Ʈ �����Ȳ Ȯ��
	QUEST	Get_CurQuest() { return m_eCurQuest; }//���� ���� ���� ����Ʈ Ȯ��
	void	IncreaseQuestProgress();//����Ʈ ��ô�� �ø���
private:
	//void Start_Tutorial_GameSpeed();
	//void Start_Tutorial_Deconstruct();
	void	Change_Quest(QUEST _eQuest);
	void	Create_MsgBox();//����Ʈ�� ���� �޽��� �ڽ� ���
	void	Create_ProgressBar();//���α׷��� �� ����

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

