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
		if ((!m_tBlockPoint[HEAD].fX) && (!m_tBlockPoint[HEAD].fY))
		{
			m_tBlockPoint[HEAD].fX = (float)ptMouse.x;
			m_tBlockPoint[HEAD].fY = (float)ptMouse.y;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if (m_eDrawDir == NO_DIR)
		{
			m_tBlockPoint[TAIL].fX = (float)ptMouse.x;
			m_tBlockPoint[TAIL].fY = (float)ptMouse.y;

			float fDistH = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
			float fDistV = abs(m_tBlockPoint[HEAD].fY - m_tBlockPoint[TAIL].fY);

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
			m_tBlockPoint[TAIL].fX = (float)ptMouse.x;
			m_tBlockPoint[TAIL].fY = m_tBlockPoint[HEAD].fY;
		}
		else if (m_eDrawDir == VERTICAL)
		{
			m_tBlockPoint[TAIL].fX = m_tBlockPoint[HEAD].fX;
			m_tBlockPoint[TAIL].fY = (float)ptMouse.y;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
	{
		float fDist(0.f);
		bool bPlus(false);

		if (m_eDrawDir == HORIZONTAL)
		{
			fDist = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
			bPlus = m_tBlockPoint[HEAD].fX < m_tBlockPoint[TAIL].fX;
		}
		else if (m_eDrawDir == VERTICAL)
		{
			fDist = abs(m_tBlockPoint[HEAD].fY - m_tBlockPoint[TAIL].fY);
			bPlus = m_tBlockPoint[HEAD].fY < m_tBlockPoint[TAIL].fY;
		}

		//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����
		int iBoxCount = int(fDist / m_fBlockSize);
		//HEAD���� TAIL���� �ڽ��� ����
		for (int i = 0; i < iBoxCount; ++i)
		{
			CObj* pBlock(nullptr);
			if (bPlus)
			{
				if (m_eDrawDir == HORIZONTAL)
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
				else if (m_eDrawDir == VERTICAL)
				{
					INFO tInfo
					{
						m_tBlockPoint[HEAD].fX,
						m_tBlockPoint[HEAD].fY + m_fBlockSize * 0.5f + i * m_fBlockSize,
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
						m_tBlockPoint[HEAD].fX - m_fBlockSize * 0.5f - i * m_fBlockSize,
						m_tBlockPoint[HEAD].fY,
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}
				else if (m_eDrawDir == VERTICAL)
				{
					INFO tInfo
					{
						m_tBlockPoint[HEAD].fX,
						m_tBlockPoint[HEAD].fY - m_fBlockSize * 0.5f - i * m_fBlockSize,
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

	return 0;
}

void CBlockMgr::Late_Update()
{
}

void CBlockMgr::Render(HDC hDC)
{
	for (auto& pLine : m_BlockList)
		pLine->Render(hDC);

	float fScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	MoveToEx(hDC, (int)m_tBlockPoint[HEAD].fX + fScrollX, (int)m_tBlockPoint[HEAD].fY + fScrollY, nullptr);
	LineTo(hDC, (int)m_tBlockPoint[TAIL].fX + fScrollX, (int)m_tBlockPoint[TAIL].fY + fScrollY);

	//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����


	float fDist(0.f);
	bool bPlus(false);

	if (m_eDrawDir == HORIZONTAL)
	{
		fDist = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
		bPlus = m_tBlockPoint[HEAD].fX < m_tBlockPoint[TAIL].fX;
	}
	else if(m_eDrawDir == VERTICAL)
	{
		fDist = abs(m_tBlockPoint[HEAD].fY - m_tBlockPoint[TAIL].fY);
		bPlus = m_tBlockPoint[HEAD].fY < m_tBlockPoint[TAIL].fY;
	}
	int iBoxCount = int(fDist / m_fBlockSize);

	//HEAD���� TAIL���� �ڽ��� ���
	for (int i = 0; i < iBoxCount ; ++i)
	{
		if (bPlus)
		{
			if (m_eDrawDir == HORIZONTAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].fX + i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].fY - m_fBlockSize * 0.5f + fScrollY),
					int(m_tBlockPoint[HEAD].fX + m_fBlockSize + i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].fY + m_fBlockSize - m_fBlockSize * 0.5f + fScrollY));
			}
			else if(m_eDrawDir == VERTICAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].fX - m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].fY + i * m_fBlockSize + fScrollY),
					int(m_tBlockPoint[HEAD].fX + m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].fY + m_fBlockSize + i * m_fBlockSize + fScrollY));
			}

		}
		else
		{
			if (m_eDrawDir == HORIZONTAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].fX - m_fBlockSize - i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].fY - m_fBlockSize * 0.5f + fScrollY),
					int(m_tBlockPoint[HEAD].fX - i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].fY + m_fBlockSize - m_fBlockSize * 0.5f + fScrollY));
			}
			else if (m_eDrawDir == VERTICAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].fX - m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].fY - i * m_fBlockSize + fScrollY),
					int(m_tBlockPoint[HEAD].fX + m_fBlockSize * 0.5f + fScrollX),
					int(m_tBlockPoint[HEAD].fY - m_fBlockSize - i * m_fBlockSize + fScrollY));
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
	HANDLE		hFile = CreateFile(L"../Data/Block_Hong.dat", // ���� ��ο� �̸��� ���
		GENERIC_WRITE,		// ���� ���� ���(GENERIC_READ)
		NULL,				// ���� ���, ������ �����ִ� ���¿��� �ٸ� ���μ����� ���� �� �� ����� ���ΰ�
		NULL,				// ���� �Ӽ�, NULL�� ��� �⺻ ���� ����
		CREATE_ALWAYS,		// ���� ���(CREATE_ALWAYS : ���Ͼ� ������ ����, ������ ���� ����, OPEN_EXISTING : ���� ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�, �ƹ��� �Ӽ��� ���� �Ϲ� ������ �������� ����
		NULL);				// ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��� NULL)

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

	MessageBox(g_hWnd, _T("Save �Ϸ�"), L"����", MB_OK);
}

void CBlockMgr::Load_Block()
{
	Release();

	HANDLE		hFile = CreateFile(L"../Data/Block_Hong.dat", // ���� ��ο� �̸��� ���
		GENERIC_READ,		// ���� ���� ���(GENERIC_READ)
		NULL,				// ���� ���, ������ �����ִ� ���¿��� �ٸ� ���μ����� ���� �� �� ����� ���ΰ�
		NULL,				// ���� �Ӽ�, NULL�� ��� �⺻ ���� ����
		OPEN_EXISTING,		// ���� ���(CREATE_ALWAYS : ���Ͼ� ������ ����, ������ ���� ����, OPEN_EXISTING : ���� ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�, �ƹ��� �Ӽ��� ���� �Ϲ� ������ �������� ����
		NULL);				// ������ ������ �Ӽ��� ������ ���ø� ����(�Ⱦ��� NULL)

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

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}
