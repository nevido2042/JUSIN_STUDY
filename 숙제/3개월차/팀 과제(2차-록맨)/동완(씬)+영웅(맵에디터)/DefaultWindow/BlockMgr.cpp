#include "pch.h"
#include "BlockMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"

CBlockMgr* CBlockMgr::m_pInstance = nullptr;

CBlockMgr::CBlockMgr()
	:m_fBlockSize(0)
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

	Load_Block();
}

int CBlockMgr::Update()
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
		//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����
		float fDist = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
		bool bRight = m_tBlockPoint[HEAD].fX < m_tBlockPoint[TAIL].fX;
		int iBoxCount = int(fDist / m_fBlockSize);
		//HEAD���� TAIL���� �ڽ��� ����
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

	return 0;
}

void CBlockMgr::Late_Update()
{
}

void CBlockMgr::Render(HDC hDC)
{
	for (auto& pLine : m_BlockList)
		pLine->Render(hDC);

	MoveToEx(hDC, (int)m_tBlockPoint[HEAD].fX + (int)CScrollMgr::Get_Instance()->Get_ScrollX(), (int)m_tBlockPoint[HEAD].fY, nullptr);
	LineTo(hDC, (int)m_tBlockPoint[TAIL].fX + (int)CScrollMgr::Get_Instance()->Get_ScrollX(), (int)m_tBlockPoint[TAIL].fY);

	//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����
	float fDist = abs(m_tBlockPoint[HEAD].fX - m_tBlockPoint[TAIL].fX);
	bool bRight = m_tBlockPoint[HEAD].fX < m_tBlockPoint[TAIL].fX;
	int iBoxCount = int(fDist / m_fBlockSize);
	//HEAD���� TAIL���� �ڽ��� ���


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

void CBlockMgr::Release()
{
	for_each(m_BlockList.begin(), m_BlockList.end(), Safe_Delete<CObj*>);
	m_BlockList.clear();
}

void CBlockMgr::Save_Block()
{
	HANDLE		hFile = CreateFile(L"../Data/Block.dat", // ���� ��ο� �̸��� ���
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

	HANDLE		hFile = CreateFile(L"../Data/Block.dat", // ���� ��ο� �̸��� ���
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

		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(Block.Get_Info()));
		//m_BlockList.push_back(CAbstractFactory<CBlock>::Create(Block.Get_Info()));
		//m_BlockList.push_back(new CBlock(Block));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

void CBlockMgr::Load_Block(LPCWSTR _FilePath)
{
	Release();

	HANDLE		hFile = CreateFile(_FilePath, // ���� ��ο� �̸��� ���
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

		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(Block.Get_Info()));
		//m_BlockList.push_back(CAbstractFactory<CBlock>::Create(Block.Get_Info()));
		//m_BlockList.push_back(new CBlock(Block));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}
