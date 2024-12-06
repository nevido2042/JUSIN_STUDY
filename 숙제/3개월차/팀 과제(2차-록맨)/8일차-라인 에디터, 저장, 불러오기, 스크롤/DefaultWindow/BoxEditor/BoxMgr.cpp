#include "pch.h"
#include "BoxMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"

CBoxMgr* CBoxMgr::m_pInstance = nullptr;

CBoxMgr::CBoxMgr()
	:m_fBlockSize(0)
{
	ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));
}

CBoxMgr::~CBoxMgr()
{
	Release();
}

void CBoxMgr::Initialize()
{
	m_fBlockSize = 50.f;
}

int CBoxMgr::Update()
{
	for (auto& pObj : m_BlockList)
		pObj->Update();

	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if ((!m_tBlockPoint[HEAD].fX) && (!m_tBlockPoint[HEAD].fY))
		{
			m_tBlockPoint[HEAD].fX = (float)ptMouse.x;
			m_tBlockPoint[HEAD].fY = (float)ptMouse.y;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		m_tBlockPoint[TAIL].fX = (float)ptMouse.x;
		m_tBlockPoint[TAIL].fY = m_tBlockPoint[HEAD].fY;

	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
	{
		//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
		float fDist = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
		bool bRight = m_tBlockPoint[HEAD].fX < m_tBlockPoint[TAIL].fX;
		int iBoxCount = int(fDist / m_fBlockSize);
		//HEAD부터 TAIL까지 박스들 생성
		for (int i = 0; i < iBoxCount; ++i)
		{
			CObj* pBlock(nullptr);
			if (bRight)
			{
				INFO tInfo
				{
					m_tBlockPoint[HEAD].fX + m_fBlockSize * 0.5f + i * m_fBlockSize,
					m_tBlockPoint[HEAD].fY,
					m_fBlockSize,
					m_fBlockSize
				};

				pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
			}
			else
			{
				INFO tInfo
				{
					m_tBlockPoint[HEAD].fX - m_fBlockSize * 0.5f - i * m_fBlockSize,
					m_tBlockPoint[HEAD].fY,
					m_fBlockSize,
					m_fBlockSize
				};

				pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
			}



			m_BlockList.push_back(pBlock);
		}

		ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		Save_Box();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		Load_Box();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	return 0;
}

void CBoxMgr::Late_Update()
{
}

void CBoxMgr::Render(HDC hDC)
{
	for (auto& pLine : m_BlockList)
		pLine->Render(hDC);

	MoveToEx(hDC, (int)m_tBlockPoint[HEAD].fX + (int)CScrollMgr::Get_Instance()->Get_ScrollX(), (int)m_tBlockPoint[HEAD].fY, nullptr);
	LineTo(hDC, (int)m_tBlockPoint[TAIL].fX + (int)CScrollMgr::Get_Instance()->Get_ScrollX(), (int)m_tBlockPoint[TAIL].fY);

	//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
	float fDist = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
	bool bRight = m_tBlockPoint[HEAD].fX < m_tBlockPoint[TAIL].fX;
	int iBoxCount = int(fDist / m_fBlockSize);
	//HEAD부터 TAIL까지 박스들 출력


	for (int i = 0; i < iBoxCount ; ++i)
	{
		if (bRight)
		{
			Rectangle(hDC,
				int(m_tBlockPoint[HEAD].fX + i * m_fBlockSize + (int)CScrollMgr::Get_Instance()->Get_ScrollX()),
				int(m_tBlockPoint[HEAD].fY - m_fBlockSize * 0.5f),
				int(m_tBlockPoint[HEAD].fX + m_fBlockSize + i * m_fBlockSize + (int)CScrollMgr::Get_Instance()->Get_ScrollX()),
				int(m_tBlockPoint[HEAD].fY + m_fBlockSize - m_fBlockSize * 0.5f));
		}
		else
		{
			
			Rectangle(hDC,
				int(m_tBlockPoint[HEAD].fX - m_fBlockSize - i * m_fBlockSize + (int)CScrollMgr::Get_Instance()->Get_ScrollX()),
				int(m_tBlockPoint[HEAD].fY - m_fBlockSize * 0.5f),
				int(m_tBlockPoint[HEAD].fX - i * m_fBlockSize + (int)CScrollMgr::Get_Instance()->Get_ScrollX()),
				int(m_tBlockPoint[HEAD].fY + m_fBlockSize - m_fBlockSize * 0.5f));
		}

	}
}

void CBoxMgr::Release()
{
	for_each(m_BlockList.begin(), m_BlockList.end(), Safe_Delete<CObj*>);
	m_BlockList.clear();
}

void CBoxMgr::Save_Box()
{
	HANDLE		hFile = CreateFile(L"../Data/Box.dat", // 파일 경로와 이름을 명시
		GENERIC_WRITE,		// 파일 접근 모드(GENERIC_READ)
		NULL,				// 공유 방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈 할 때 허용할 것인가
		NULL,				// 보안 속성, NULL인 경우 기본 보안 상태
		CREATE_ALWAYS,		// 생성 방식(CREATE_ALWAYS : 파일어 없으면 생성, 있으면 덮어 쓰기, OPEN_EXISTING : 파일 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성, 아무런 속성이 없는 일반 파일의 형식으로 생성
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte(0);

	for (auto& pBlock : m_BlockList)
	{
		WriteFile(hFile, pBlock, sizeof(CBlock), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CBoxMgr::Load_Box()
{
	Release();

	HANDLE		hFile = CreateFile(L"../Data/Box.dat", // 파일 경로와 이름을 명시
		GENERIC_READ,		// 파일 접근 모드(GENERIC_READ)
		NULL,				// 공유 방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈 할 때 허용할 것인가
		NULL,				// 보안 속성, NULL인 경우 기본 보안 상태
		OPEN_EXISTING,		// 생성 방식(CREATE_ALWAYS : 파일어 없으면 생성, 있으면 덮어 쓰기, OPEN_EXISTING : 파일 있을 경우에만 열기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성, 아무런 속성이 없는 일반 파일의 형식으로 생성
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일(안쓰니 NULL)

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}

	DWORD	dwByte(0);
	CBlock	Block;

	while (true)
	{
		bool bResult = ReadFile(hFile, &Block, sizeof(CBlock), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_BlockList.push_back(CAbstractFactory<CBlock>::Create(Block.Get_Info()));
		//m_BlockList.push_back(new CBlock(Block));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}
