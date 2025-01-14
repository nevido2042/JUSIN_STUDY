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
	
	// Step 1: Create a new font
	HFONT hFont = CreateFont(
		18,                        // Height of font
		0,                         // Width of font (0 means default)
		0,                         // Angle of text rotation (0 for horizontal)
		0,                         // Angle of text rotation (0 for horizontal)
		FW_NORMAL,                 // Weight (normal font)
		FALSE,                     // Italic
		FALSE,                     // Underline
		FALSE,                     // Strikeout
		ANSI_CHARSET,              // Character set
		OUT_DEFAULT_PRECIS,        // Output precision
		CLIP_DEFAULT_PRECIS,       // Clipping precision
		DEFAULT_QUALITY,           // Quality
		DEFAULT_PITCH | FF_SWISS,  // Pitch and family
		L"맑은 고딕"                   // Font name
	);

	// Step 2: Select the font into the device context
	HFONT hOldFont =  (HFONT)SelectObject(hDC, hFont);

	switch (eCurQuest)
	{
	case CTutorialMgr::QUEST_GAMESPEED:
		/*DrawText(
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
		break;*/

		// Step 3: Draw the text with the new font
		DrawText(
			hDC,
			L" \n"
			L" 퀘스트:\n"
			L" 게임 속도를 변경하거나 멈추세요.\n"
			L" \n"
			L" 숫자 키 1, 2, 3 눌러서 배속 조절\n"
			L" SPACE 키를 눌러서 일시 정지\n",
			-1,
			&rect,
			DT_NOCLIP
		);

		break;
	case CTutorialMgr::QUEST_DECONSTRUCT:
		DrawText(
			hDC,
			L" \n"
			L" 퀘스트:\n"
			L" 강철 벽을 해체해서\n"
			L" 강철을 얻으세요.\n"
			L" \n"
			L" 구상 -> 명령 -> 해체\n"
			L" 마우스 왼쪽 버튼 드래그로\n"
			L" 해체 할 강철 벽 지정\n"
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
			L" 퀘스트:\n"
			L" 벌목 해서 원목을 얻으세요.\n"
			L" \n"
			L" 구상 -> 명령 -> 벌목\n"
			L" 마우스 왼쪽 버튼 드래그로\n"
			L" 벌목 할 나무 지정\n"
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
			L" 퀘스트:\n"
			L" 적의 이동을 제한 하기 위해\n"
			L" 철벽을 건설하세요.\n"
			L" \n"
			L" 구상 -> 구조물 -> 철벽\n"
			L" 비어있는 땅에\n"
			L" 클릭 또는 드래그하여\n"
			L" 원하는 위치에 건설\n"
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
			L" 퀘스트:\n"
			L" 추위를 견디기 위해\n"
			L" 모닥불을 설치하세요.\n"
			L" \n"
			L" 구상 -> 구조물 -> 모닥불\n"
			L" 비어있는 땅에\n"
			L" 클릭 또는 드래그하여\n"
			L" 설치 하고 싶은 위치에 설치\n"
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
			L" 퀘스트:\n"
			L" 적과의 전투를 위해\n"
			L" 림들을 소집하세요.\n"
			L" \n"
			L" 원하는 림을 선택 후\n"
			L" 'R' 키로 소집 상태로 전환\n"
			L" \n"
			L" 소집상태로 전환 후\n"
			L" 원하는 곳에 림 배치\n"
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
			L" 퀘스트:\n"
			L" 적들이 몰려 옵니다.\n"
			L" 적과 싸워 승리하세요.\n"
			L" \n"
			L" 오인 사격 방지를 위해\n"
			L" 림을 적절히 배치하세요.\n"
			L" \n"
			L" 지네로봇을 처치하면\n"
			L" 우주선 설계도를 얻을 수 있습니다.\n"
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
			L" 퀘스트:\n"
			L" \n"
			L" 지네로봇을 처치하여.\n"
			L" 우주선 설계도를 얻었습니다.\n"
			L" \n"
			L" 우주선 설계도로.\n"
			L" 우주선을 건설하세요.\n"
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
			L" 퀘스트:\n"
			L" \n"
			L" 림을 우주선에 태워\n"
			L" 이 행성을 탈출 하세요.\n"
			L" \n"
			L" 림을 클릭 후.\n"
			L" 우주선을 클릭하면.\n"
			L" 림이 우주선에 탑승합니다.\n"
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
	
	// Step 4: Restore the old font
	SelectObject(hDC, hOldFont);

	// Optional: Step 5: Delete the created font when done
	DeleteObject(hFont);
}
