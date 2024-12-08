#include "pch.h"
#include "BlockMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"

CBlockMgr* CBlockMgr::m_pInstance = nullptr;

CBlockMgr::CBlockMgr()
	:m_fBlockSize(0), m_eDrawDir(NO_DIR),
	m_iWidth(0), m_iHeight(0)
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

	m_iWidth = 50;
	m_iHeight = 50;
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
			m_tBlockPoint[TAIL].x = ptMouse.x;
			m_tBlockPoint[TAIL].y = ptMouse.y;

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

		//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����
		const int iBoxCount = int(fDist / m_fBlockSize);
		m_iUndo_Stack.push_back(iBoxCount);

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
						float(m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5 + i * m_fBlockSize),
						float(m_tBlockPoint[HEAD].y + m_fBlockSize * 0.5),
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}
				else if (m_eDrawDir == VERTICAL)
				{
					INFO tInfo
					{
						float(m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5f),
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
						(float)m_tBlockPoint[HEAD].y + m_fBlockSize * 0.5f,
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}
				else if (m_eDrawDir == VERTICAL)
				{
					INFO tInfo
					{
						(float)m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5f,
						float(m_tBlockPoint[HEAD].y - m_fBlockSize * 0.5f - i * m_fBlockSize),
						m_fBlockSize,
						m_fBlockSize
					};
					pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
				}		
			}

			m_BlockList.push_back(pBlock);
		}

		//�ϳ��� ���� �� ���� ���̸� �� ���� �� ĭ�� ����
		if (iBoxCount == 0)
		{
			CObj* pBlock(nullptr);
			INFO tInfo
			{
				float(m_tBlockPoint[HEAD].x + m_fBlockSize * 0.5f),
				(float)m_tBlockPoint[HEAD].y + m_fBlockSize * 0.5f,
				m_fBlockSize,
				m_fBlockSize
			};
			pBlock = CAbstractFactory<CBlock>::Create(&tInfo);
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
	//��ũ��
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//�׸��� �׸���
	for (int i = 0; i < m_iWidth; ++i)
	{
		MoveToEx(hDC, i * m_fBlockSize + fScrollX , 0 + fScrollY, nullptr);
		LineTo(hDC, i * m_fBlockSize + fScrollX, m_iHeight * m_fBlockSize + fScrollY);
	}
	for (int i = 0; i < m_iHeight; ++i)
	{
		MoveToEx(hDC, 0 + fScrollX, i * m_fBlockSize + fScrollY, nullptr);
		LineTo(hDC, m_iWidth * m_fBlockSize + fScrollX, i * m_fBlockSize + fScrollY);
	}

	//�ڽ� �ν��Ͻ� ���
	for (auto& pLine : m_BlockList)
		pLine->Render(hDC);

	//�� �׸���
	//MoveToEx(hDC, int(m_tBlockPoint[HEAD].x + fScrollX), int(m_tBlockPoint[HEAD].y + fScrollY), nullptr);
	//LineTo(hDC, int(m_tBlockPoint[TAIL].x + fScrollX), int(m_tBlockPoint[TAIL].y + fScrollY));

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
	//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����
	int iBoxCount = int(fDist / m_fBlockSize);

	//HEAD���� TAIL���� �ڽ��� ���
	for (int i = 0; i < iBoxCount ; ++i)
	{
		if (bPlus)
		{
			if (m_eDrawDir == HORIZONTAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x + i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + fScrollY),
					int(m_tBlockPoint[HEAD].x + m_fBlockSize + i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + m_fBlockSize + fScrollY));
			}
			else if(m_eDrawDir == VERTICAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x + fScrollX),
					int(m_tBlockPoint[HEAD].y + i * m_fBlockSize + fScrollY),
					int(m_tBlockPoint[HEAD].x + m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + m_fBlockSize + i * m_fBlockSize + fScrollY));
			}

		}
		else
		{
			if (m_eDrawDir == HORIZONTAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x - m_fBlockSize - i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + fScrollY),
					int(m_tBlockPoint[HEAD].x - i * m_fBlockSize + fScrollX),
					int(m_tBlockPoint[HEAD].y + m_fBlockSize + fScrollY));
			}
			else if (m_eDrawDir == VERTICAL)
			{
				Rectangle(hDC,
					int(m_tBlockPoint[HEAD].x + fScrollX),
					int(m_tBlockPoint[HEAD].y - i * m_fBlockSize + fScrollY),
					int(m_tBlockPoint[HEAD].x + m_fBlockSize + fScrollX),
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
