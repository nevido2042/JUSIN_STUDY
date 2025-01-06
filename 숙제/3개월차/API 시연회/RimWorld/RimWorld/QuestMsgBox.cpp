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

	RECT rect = { m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom }; // 텍스트를 출력할 영역
	
	CTutorialMgr::QUEST eCurQuest = CTutorialMgr::Get_Instance()->Get_CurQuest();
	
	switch (eCurQuest)
	{
	case CTutorialMgr::QUEST_GAMESPEED:
		DrawText(
			hDC,
			L" \n"
			L" 퀘스트:\n"
			L" 게임 속도를 변경하거나 멈추세요.\n"
			L" \n"
			L" 숫자 키 1, 2, 3 눌러서 배속 조절\n"
			L" SPACE 키를 눌러서 일시 정지\n"
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
