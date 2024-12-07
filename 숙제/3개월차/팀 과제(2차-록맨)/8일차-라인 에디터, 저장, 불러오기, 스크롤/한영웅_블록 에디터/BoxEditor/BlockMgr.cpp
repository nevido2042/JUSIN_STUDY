#include "pch.h"
#include "BlockMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"

CBlockMgr* CBlockMgr::m_pInstance = nullptr;

CBlockMgr::CBlockMgr()
	:m_fBlockSize(0), m_eDrawDir(NO_DIR)
{
	ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));
}

CBlockMgr::~CBlockMgr()
{
	Release();
}

void CBlockMgr::Initialize()
{
	m_fBlockSize = 50.f;
}

int CBlockMgr::Update()
{
	for (auto& pObj : m_BlockList)
		pObj->Update();

	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if ((!m_tBlockPoint[HEAD].x) && (!m_tBlockPoint[HEAD].y))
		{
			m_tBlockPoint[HEAD].x = ptMouse.x - (ptMouse.x % long(m_fBlockSize));
			m_tBlockPoint[HEAD].y = ptMouse.y - (ptMouse.y % long(m_fBlockSize));
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if (m_eDrawDir == NO_DIR)
		{
			m_tBlockPoint[TAIL].x = ptMouse.x;// - (ptMouse.x % long(m_fBlockSize));
			m_tBlockPoint[TAIL].y = ptMouse.y;// - (ptMouse.y % long(m_fBlockSize));

			float fDistH = (float)abs(m_tBlockPoint[HEAD].x - m_tBlockPoint[TAIL].x);
			float fDistV = (float)abs(m_tBlockPoint[HEAD].y - m_tBlockPoint[TAIL].y);

			if (fDistH > m_fBlockSize)
			{
				m_eDrawDir = HORIZONTAL;
			}

			if (fDistV > m_fBlockSize)
			{
				m_eDrawDir = VERTICAL;
			}
		}


		if (m_eDrawDir == HORIZONTAL)
		{
			m_tBlockPoint[TAIL].x = ptMouse.x;
			m_tBlockPoint[TAIL].y = m_tBlockPoint[HEAD].y;
		}
		else if (m_eDrawDir == VERTICAL)
		{
			m_tBlockPoint[TAIL].x = m_tBlockPoint[HEAD].x;
			m_tBlockPoint[TAIL].y = ptMouse.y;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
	{
		float fDist(0.f);
		bool bPlus(false);

		if (m_eDrawDir == HORIZONTAL)
		{
			fDist = (float)abs(m_tBlockPoint[HEAD].x - m_tBlockPoint[TAIL].x);
			bPlus = m_tBlockPoint[HEAD].x < m_tBlockPoint[TAIL].x;
		}
		else if (m_eDrawDir == VERTICAL)
		{
			fDist = (float)abs(m_tBlockPoint[HEAD].y - m_tBlockPoint[TAIL].y);
			bPlus = m_tBlockPoint[HEAD].y < m_tBlockPoint[TAIL].y;
		}

		//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
		int iBoxCount = int(fDist / m_fBlockSize);
		m_iUndo_Stack.push_back(iBoxCount);

		//HEAD부터 TAIL까지 박스들 생성
		for (int i = 0; i < iBoxCount; ++i)
		{
			CObj* pBlock(nullptr);
			if (bPlus)
			{
				if (m_eDrawDir == HORIZONTAL)
				{
					INFO tInfo
					{
						float(m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5f + i * m_fBlockSize),
						float(m_tBlockPoint[HEAD].y),
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}
				else if (m_eDrawDir == VERTICAL)
				{
					INFO tInfo
					{
						float(m_tBlockPoint[HEAD].x),
						float(m_tBlockPoint[HEAD].y + m_fBlockSize * 0.5f + i * m_fBlockSize),
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}
			}
			else
			{
				if (m_eDrawDir == HORIZONTAL)
				{
					INFO tInfo
					{
						float(m_tBlockPoint[HEAD].x - m_fBlockSize * 0.5f - i * m_fBlockSize),
						(float)m_tBlockPoint[HEAD].y,
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}
				else if (m_eDrawDir == VERTICAL)
				{
					INFO tInfo
					{
						(float)m_tBlockPoint[HEAD].x,
						float(m_tBlockPoint[HEAD].y - m_fBlockSize * 0.5f - i * m_fBlockSize),
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}		
			}
			m_BlockList.push_back(pBlock);
		}

		ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));

		m_eDrawDir = NO_DIR;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		Save_Block();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		Load_Block();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		if (!m_iUndo_Stack.empty() && CKeyMgr::Get_Instance()->Key_Pressing(VK_LCONTROL))
		{
			for (int i = 0; i < m_iUndo_Stack.back(); ++i)
			{
				Safe_Delete<CObj*>(m_BlockList.back());
				m_BlockList.pop_back();
			}
			m_iUndo_Stack.pop_back();
		}
	}

	return 0;
}

void CBlockMgr::Late_Update()
{
}

void CBlockMgr::Render(HDC hDC)
{
	for (auto& pLine : m_BlockList)
		pLine->Render(hDC);

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	MoveToEx(hDC, int(m_tBlockPoint[HEAD].x + fScrollX), int(m_tBlockPoint[HEAD].y + fScrollY), nullptr);
	LineTo(hDC, int(m_tBlockPoint[TAIL].x + fScrollX), int(m_tBlockPoint[TAIL].y + fScrollY));

	float fDist(0.f);
	bool bPlus(false);

	if (m_eDrawDir == HORIZONTAL)
	{
		fDist = (float)abs(m_tBlockPoint[HEAD].x - m_tBlockPoint[TAIL].x);
		bPlus = m_tBlockPoint[HEAD].x < m_tBlockPoint[TAIL].x;
	}
	else if(m_eDrawDir == VERTICAL)
	{
		fDist = (float)abs(m_tBlockPoint[HEAD].y - m_tBlockPoint[TAIL].y);
		bPlus = m_tBlockPoint[HEAD].y < m_tBlockPoint[TAIL].y;
	}
	//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
	int iBoxCount = int(fDist / m_fBlockSize);

	//HEAD부터 TAIL까지 박스들 출력
	for (int i = 0; i < iBoxCount ; ++i)
	{
		if (bPlus)
		{
			if (m_eDrawDir == HORIZONTAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x + i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y - m_fBlockSize * 0.5f + fScrollY),
					int(m_tBlockPoint[HEAD].x + m_fBlockSize + i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + m_fBlockSize - m_fBlockSize * 0.5f + fScrollY));
			}
			else if(m_eDrawDir == VERTICAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x - m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].y + i * m_fBlockSize + fScrollY),
					int(m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].y + m_fBlockSize + i * m_fBlockSize + fScrollY));
			}

		}
		else
		{
			if (m_eDrawDir == HORIZONTAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x - m_fBlockSize - i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y - m_fBlockSize * 0.5f + fScrollY),
					int(m_tBlockPoint[HEAD].x - i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + m_fBlockSize - m_fBlockSize * 0.5f + fScrollY));
			}
			else if (m_eDrawDir == VERTICAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x - m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].y - i * m_fBlockSize + fScrollY),
					int(m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].y - m_fBlockSize - i * m_fBlockSize + fScrollY));
			}

		}

	}
}

void CBlockMgr::Release()
{
	for_each(m_BlockList.begin(), m_BlockList.end(), Safe_Delete<CObj*>);
	m_BlockList.clear();
}

void CBlockMgr::Save_Block()
{
	HANDLE		hFile = CreateFile(L"../Data/Block.dat", // 파일 경로와 이름을 명시
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

void CBlockMgr::Load_Block()
{
	Release();

	HANDLE		hFile = CreateFile(L"../Data/Block.dat", // 파일 경로와 이름을 명시
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
