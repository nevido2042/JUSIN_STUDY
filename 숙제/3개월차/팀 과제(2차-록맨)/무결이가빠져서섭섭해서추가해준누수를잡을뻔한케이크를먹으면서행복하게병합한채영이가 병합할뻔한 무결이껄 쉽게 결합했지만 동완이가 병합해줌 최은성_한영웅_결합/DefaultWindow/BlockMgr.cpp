#include "pch.h"
#include "BlockMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"

#include "Block.h"
//#include "Monster.h"

#include "Block_Ice.h"
#include "Block_Fire.h"
#include "Block_Elec.h"
#include "Block_Cut.h"
#include "Block_Gut.h"

CBlockMgr* CBlockMgr::m_pInstance = nullptr;

CBlockMgr::CBlockMgr()
	:m_fBlockSize(0), m_eDrawDir(NO_DIR),
	m_iWidth(0), m_iHeight(0), m_eBlockType(BLOCKTYPE_END),//(2)�̰� �³�?
	m_fSpeed(0.f)
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

	m_eBlockType = BLOCK_ICE;

	m_fSpeed = 10.f;

	//Load_Block();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_ice.bmp", L"Tile_Ice");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_fire.bmp", L"Tile_Fire");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_elec.bmp", L"Tile_Elec");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_cut.bmp", L"Tile_Cut");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Rock_Man/tile_gut.bmp", L"Tile_Gut");

}

int CBlockMgr::Update()
{
	/*for (auto& pObj : ObjMgr)
		pObj->Update();*/

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
		//�ǵ����� ����Ʈ ����
		list<CObj*> UndoList;

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

					pBlock = Create_Block(&tInfo);

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
					pBlock = Create_Block(&tInfo);
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
					pBlock = Create_Block(&tInfo);
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
					pBlock = Create_Block(&tInfo);
				}
			}

			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pBlock);
			//m_ObjList.push_back(pBlock);

			//�ǵ��� ����Ʈ�� �߰�
			//m_UndoList.push_back(pBlock);


			UndoList.push_back(pBlock);
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

			pBlock = Create_Block(&tInfo);
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pBlock);
			//m_ObjList.push_back(pBlock);

			//�ǵ��� ����Ʈ�� �߰�
			UndoList.push_back(pBlock);
		}

		//������ ����Ʈ�� �ǵ����� ���ÿ� Ǫ��
		m_UndoStack.push(UndoList);

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
	//�� ����
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		m_eBlockType = BLOCK_ICE;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('2'))
	{
		m_eBlockType = BLOCK_FIRE;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		m_eBlockType = BLOCK_ELEC;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		m_eBlockType = BLOCK_CUT;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('5'))
	{
		m_eBlockType = BLOCK_GUT;
	}

	//ȭ�� �����̱�
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);

	//�ǵ����� Ctrl+Z
	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{

		if (!m_UndoStack.empty() && CKeyMgr::Get_Instance()->Key_Pressing(VK_LCONTROL))
		{


			for (auto i : m_UndoStack.top())
			{
				i->Set_Dead();
			}

			m_UndoStack.pop();
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
		MoveToEx(hDC, int(i * m_fBlockSize + fScrollX), int(0 + fScrollY), nullptr);
		LineTo(hDC, int(i * m_fBlockSize + fScrollX), int(m_iHeight * m_fBlockSize + fScrollY));
	}
	for (int i = 0; i < m_iHeight; ++i)
	{
		MoveToEx(hDC, int(0 + fScrollX), int(i * m_fBlockSize + fScrollY), nullptr);
		LineTo(hDC, int(m_iWidth * m_fBlockSize + fScrollX), int(i * m_fBlockSize + fScrollY));
	}

	Rectangle(hDC,
		0,
		0,
		WINCX,
		m_fBlockSize * 1.5f);

	HDC		hMemDC;
	switch (m_eBlockType)
	{
	case BLOCK_ICE:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Ice");

		BitBlt(hDC,						// ���� ���� DC
			0,	// ���� ���� ��ġ ��ǥ X, Y	
			0,
			(int)m_fBlockSize,			// ���� ���� �̹����� ����, ����
			(int)m_fBlockSize,
			hMemDC,						// ������ �̹��� DC
			35,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
			163,
			SRCCOPY);
		break;
	case BLOCK_FIRE:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Fire");

		BitBlt(hDC,						// ���� ���� DC
			0,	// ���� ���� ��ġ ��ǥ X, Y	
			0,
			(int)m_fBlockSize,			// ���� ���� �̹����� ����, ����
			(int)m_fBlockSize,
			hMemDC,						// ������ �̹��� DC
			206,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
			3,
			SRCCOPY);
		break;

	case BLOCK_ELEC:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Elec");

		BitBlt(hDC,						// ���� ���� DC
			0,	// ���� ���� ��ġ ��ǥ X, Y	
			0,
			(int)m_fBlockSize,			// ���� ���� �̹����� ����, ����
			(int)m_fBlockSize,
			hMemDC,						// ������ �̹��� DC
			206,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
			3,
			SRCCOPY);
		break;
	case BLOCK_CUT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Cut");

		BitBlt(hDC,						// ���� ���� DC
			0,	// ���� ���� ��ġ ��ǥ X, Y	
			0,
			(int)m_fBlockSize,			// ���� ���� �̹����� ����, ����
			(int)m_fBlockSize,
			hMemDC,						// ������ �̹��� DC
			206,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
			3,
			SRCCOPY);
		break;
	case BLOCK_GUT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Gut");

		BitBlt(hDC,						// ���� ���� DC
			0,	// ���� ���� ��ġ ��ǥ X, Y	
			0,
			(int)m_fBlockSize,			// ���� ���� �̹����� ����, ����
			(int)m_fBlockSize,
			hMemDC,						// ������ �̹��� DC
			2,							// ��Ʈ�� ��� ���� ��ǥ(Left, top)
			3,
			SRCCOPY);
		break;
	}


	//�ڽ� �ν��Ͻ� ���
	//for (auto& pLine : m_ObjList)
		//pLine->Render(hDC);

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
	else if (m_eDrawDir == VERTICAL)
	{
		fDist = (float)abs(m_tBlockPoint[HEAD].y - m_tBlockPoint[TAIL].y);
		bPlus = m_tBlockPoint[HEAD].y < m_tBlockPoint[TAIL].y;
	}
	//HEAD��TAIL�� �Ÿ� / �ڽ� ũ�� = �ڽ� ����
	int iBoxCount = int(fDist / m_fBlockSize);

	//HEAD���� TAIL���� �ڽ��� ���
	for (int i = 0; i < iBoxCount; ++i)
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
			else if (m_eDrawDir == VERTICAL)
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
	//for_each(m_ObjList.begin(), m_ObjList.end(), Safe_Delete<CObj*>);
	//m_ObjList.clear();
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

	//�� �� ������Ʈ ����Ʈ ���� ��ü�� CObj�� �����Ѵ�.
	for (auto pObj : CObjMgr::Get_Instance()->Get_ObjList()[OBJ_BLOCK])
	{
		WriteFile(hFile,
			pObj,
			sizeof(CBlock),
			&dwByte, nullptr);
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
	CBlock  Block;
	//ReadFile�ؼ� � ������Ʈ���� �Ǵ��Ѵ���(CObj�� Ÿ�� ��������� �ּ� �Ǵ��ؾ� �ڴ�.)
	//�ش� ����Ʈ�� �־�����Ѵ�.
	while (true)
	{
		//�� �ϳ��� �����´�.
		bool bResult = ReadFile(hFile,
			&Block,
			sizeof(CBlock),
			&dwByte, nullptr);
		//�о�°� ������ ��
		if (0 == dwByte)
			break;

		CObj* pNewObj(nullptr);
		//�� Ÿ�� Ȯ��
		switch (Block.Get_BlockType())
		{
		case BLOCK_ICE:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Ice>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_FIRE:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Fire>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_ELEC:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Elec>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_CUT:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Cut>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_GUT:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Gut>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		}
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
	CBlock  Block;
	//ReadFile�ؼ� � ������Ʈ���� �Ǵ��Ѵ���(CObj�� Ÿ�� ��������� �ּ� �Ǵ��ؾ� �ڴ�.)
	//�ش� ����Ʈ�� �־�����Ѵ�.
	while (true)
	{
		//�� �ϳ��� �����´�.
		bool bResult = ReadFile(hFile,
			&Block,
			sizeof(CBlock),
			&dwByte, nullptr);
		//�о�°� ������ ��
		if (0 == dwByte)
			break;

		CObj* pNewObj(nullptr);
		//�� Ÿ�� Ȯ��
		switch (Block.Get_BlockType())
		{
		case BLOCK_ICE:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Ice>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_FIRE:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Fire>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_ELEC:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Elec>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_CUT:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Cut>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_GUT:
			//Info�� ��� �� ����
			pNewObj = CAbstractFactory<CBlock_Gut>::Create(OBJ_BLOCK, Block.Get_Info());
			//�ش� ����Ʈ�� ������Ʈ �߰�
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		}
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

CObj* CBlockMgr::Create_Block(INFO* _tInfo)
{
	CObj* pObj(nullptr);
	switch (m_eBlockType)
	{
	case BLOCK_ICE:
		pObj = CAbstractFactory<CBlock_Ice>::Create(OBJ_BLOCK, _tInfo);
		break;
	case BLOCK_FIRE:
		pObj = CAbstractFactory<CBlock_Fire>::Create(OBJ_BLOCK, _tInfo);
		break;
	case BLOCK_ELEC:
		pObj = CAbstractFactory<CBlock_Elec>::Create(OBJ_BLOCK, _tInfo);
		break;
	case BLOCK_CUT:
		pObj = CAbstractFactory<CBlock_Cut>::Create(OBJ_BLOCK, _tInfo);
		break;
	case BLOCK_GUT:
		pObj = CAbstractFactory<CBlock_Gut>::Create(OBJ_BLOCK, _tInfo);
		break;
	}

	return pObj;
}
