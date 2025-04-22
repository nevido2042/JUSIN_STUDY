#include "pch.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
	ZeroMemory(m_tLinePoint, sizeof(m_tLinePoint));
}

CLineMgr::~CLineMgr()
{
	Release();
}

bool CLineMgr::Collision_Line(float _fX, float* pY)
{
	if(m_Linelist.empty())
		return false;


	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_Linelist)
	{
		if (_fX >= pLine->Get_Info()->tLPoint.fX &&
			_fX < pLine->Get_Info()->tRPoint.fX)
		{
			pTargetLine = pLine;
		}
	}

	if (!pTargetLine)
		return false;

	float	x1 = pTargetLine->Get_Info()->tLPoint.fX;
	float	y1 = pTargetLine->Get_Info()->tLPoint.fY;
	float	x2 = pTargetLine->Get_Info()->tRPoint.fX;
	float	y2 = pTargetLine->Get_Info()->tRPoint.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

	return true;
}

void CLineMgr::Initialize()
{
	
}

int CLineMgr::Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	
	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		// ���� �� ��

		if ((!m_tLinePoint[LEFT].fX) && (!m_tLinePoint[LEFT].fY))
		{
			m_tLinePoint[LEFT].fX = (float)ptMouse.x;
			m_tLinePoint[LEFT].fY = (float)ptMouse.y;
		}

		else
		{
			m_tLinePoint[RIGHT].fX = (float)ptMouse.x;
			m_tLinePoint[RIGHT].fY = (float)ptMouse.y;

			m_Linelist.push_back(new CLine(m_tLinePoint[LEFT], m_tLinePoint[RIGHT]));

			m_tLinePoint[LEFT].fX = m_tLinePoint[RIGHT].fX;
			m_tLinePoint[LEFT].fY = m_tLinePoint[RIGHT].fY;
		}

	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		Save_Line();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
	{
		Load_Line();
		return 0;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	return 0;
}

void CLineMgr::Late_Update()
{
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& pLine : m_Linelist)
		pLine->Render(hDC);
}

void CLineMgr::Release()
{
	for_each(m_Linelist.begin(), m_Linelist.end(), Safe_Delete<CLine*>);
	m_Linelist.clear();
}

void CLineMgr::Save_Line()
{
	HANDLE		hFile = CreateFile(L"../Data/Line.dat", // ���� ��ο� �̸��� ���
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

	for (auto& pLine : m_Linelist)
	{
		WriteFile(hFile, pLine->Get_Info(), sizeof(LINE), &dwByte, nullptr);
	}

	CloseHandle(hFile);
		
	MessageBox(g_hWnd, _T("Save �Ϸ�"), L"����", MB_OK);
}

void CLineMgr::Load_Line()
{
	Release();

	HANDLE		hFile = CreateFile(L"../Data/Line.dat", // ���� ��ο� �̸��� ���
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
	LINE	tLine{};

	while (true)
	{
		bool bResult = ReadFile(hFile, &tLine, sizeof(LINE), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_Linelist.push_back(new CLine(tLine));
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}
