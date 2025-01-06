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
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ��ö ���� ��ü�ؼ�\n"
			L" ��ö�� ��������.\n"
			L" \n"
			L" ���� -> ��� -> ��ü\n"
			L" ���콺 ���� ��ư �巡�׷�\n"
			L" ��ü �� ��ö �� ����\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_LOGGING:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ���� �ؼ� ������ ��������.\n"
			L" \n"
			L" ���� -> ��� -> ����\n"
			L" ���콺 ���� ��ư �巡�׷�\n"
			L" ���� �� ���� ����\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_CONSTRUCT_WALL:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ���� �̵��� ���� �ϱ� ����\n"
			L" ö���� �Ǽ��ϼ���.\n"
			L" \n"
			L" ���� -> ������ -> ö��\n"
			L" ����ִ� ���� Ŭ�� �Ǵ� �巡���Ͽ�\n"
			L" �Ǽ� �ϰ� ���� ��ġ�� �Ǽ�\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_CONSTRUCT_CAMPFIRE:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ������ �ߵ�� ����\n"
			L" ��ں��� ��ġ�ϼ���.\n"
			L" \n"
			L" ���� -> ������ -> ��ں�\n"
			L" ����ִ� ����\n"
			L" Ŭ�� �Ǵ� �巡���Ͽ�\n"
			L" ��ġ �ϰ� ���� ��ġ�� ��ġ\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_DRAFTE:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ������ ������ ����\n"
			L" ������ �����ϼ���.\n"
			L" \n"
			L" ���ϴ� ���� ���� ��\n"
			L" 'R' Ű�� ���� ���·� ��ȯ\n"
			L" \n"
			L" �������·� ��ȯ ��\n"
			L" ���ϴ� ���� �� ��ġ\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_BATTLE:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" ������ ���� �ɴϴ�.\n"
			L" ���� �ο� �¸��ϼ���.\n"
			L" \n"
			L" ���� ��� ������ ����\n"
			L" ���� ������ ��ġ�ϼ���\n"
			L" \n"
			L" ���׷κ��� óġ�ϸ�\n"
			L" ���ּ� ���赵�� ���� �� �ֽ��ϴ�.\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_CONSTRUCT_SHIP:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" \n"
			L" ���׷κ��� óġ�Ͽ�.\n"
			L" ���ּ� ���赵�� ������ϴ�.\n"
			L" \n"
			L" ���ּ� ���赵��.\n"
			L" ���ּ��� �Ǽ��ϼ���.\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_ESCAPE:
		DrawText(
			hDC,
			L" \n"
			L" ����Ʈ:\n"
			L" \n"
			L" ���� ���ּ��� �¿�\n."
			L" �� �༺�� Ż�� �ϼ���.\n"
			L" \n"
			L" ���� Ŭ�� ��.\n"
			L" ���ּ��� Ŭ���ϸ�.\n"
			L" ���� ���ּ��� ž���մϴ�.\n"
			,
			-1,
			&rect,
			DT_NOCLIP
		);
		break;
	case CTutorialMgr::QUEST_END:
		break;
	default:
		break;
	}
}
