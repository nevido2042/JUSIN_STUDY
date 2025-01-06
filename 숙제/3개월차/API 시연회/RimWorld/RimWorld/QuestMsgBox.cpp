#include "pch.h"
#include "QuestMsgBox.h"
#include "TutorialMgr.h"

CQuestMsgBox::CQuestMsgBox()
{

}

CQuestMsgBox::~CQuestMsgBox()
{
	Release();
}

void CQuestMsgBox::Initialize()
{
	CMsgBox::Initialize();
}

void CQuestMsgBox::Render(HDC hDC)
{
	CMsgBox::Render(hDC);

	RECT rect = { m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom }; // �ؽ�Ʈ�� ����� ����
	
	CTutorialMgr::QUEST eCurQuest = CTutorialMgr::Get_Instance()->Get_CurQuest();
	
	switch (eCurQuest)
	{
	case CTutorialMgr::QUEST_GAMESPEED:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ���� �ӵ��� �����ϰų� ���߼���.\n"
			L" \n"
			L" ���� Ű 1, 2, 3 ������ ��� ����\n"
			L" SPACE Ű�� ������ �Ͻ� ����\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_DECONSTRUCT:
		break;
	case CTutorialMgr::QUEST_END:
		break;
	default:
		break;
	}
}
