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
	m_iWidth(0), m_iHeight(0), m_eBlockType(BLOCKTYPE_END),//(2)이게 맞나?
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
		//되돌리기 리스트 생성
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

		//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
		const int iBoxCount = int(fDist / m_fBlockSize);

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

			//되돌릴 리스트에 추가
			//m_UndoList.push_back(pBlock);


			UndoList.push_back(pBlock);
		}



		//하나도 만들 수 없는 길이면 그 때는 그 칸만 생성
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

			//되돌릴 리스트에 추가
			UndoList.push_back(pBlock);
		}

		//생성된 리스트를 되돌리기 스택에 푸시
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
	//블럭 선택
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

	//화면 움직이기
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);

	//되돌리기 Ctrl+Z
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
	//스크롤
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	//그리드 그리기
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

		BitBlt(hDC,						// 복사 받을 DC
			0,	// 복사 받을 위치 좌표 X, Y	
			0,
			(int)m_fBlockSize,			// 복사 받을 이미지의 가로, 세로
			(int)m_fBlockSize,
			hMemDC,						// 복사할 이미지 DC
			35,							// 비트맵 출력 시작 좌표(Left, top)
			163,
			SRCCOPY);
		break;
	case BLOCK_FIRE:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Fire");

		BitBlt(hDC,						// 복사 받을 DC
			0,	// 복사 받을 위치 좌표 X, Y	
			0,
			(int)m_fBlockSize,			// 복사 받을 이미지의 가로, 세로
			(int)m_fBlockSize,
			hMemDC,						// 복사할 이미지 DC
			206,							// 비트맵 출력 시작 좌표(Left, top)
			3,
			SRCCOPY);
		break;

	case BLOCK_ELEC:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Elec");

		BitBlt(hDC,						// 복사 받을 DC
			0,	// 복사 받을 위치 좌표 X, Y	
			0,
			(int)m_fBlockSize,			// 복사 받을 이미지의 가로, 세로
			(int)m_fBlockSize,
			hMemDC,						// 복사할 이미지 DC
			206,							// 비트맵 출력 시작 좌표(Left, top)
			3,
			SRCCOPY);
		break;
	case BLOCK_CUT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Cut");

		BitBlt(hDC,						// 복사 받을 DC
			0,	// 복사 받을 위치 좌표 X, Y	
			0,
			(int)m_fBlockSize,			// 복사 받을 이미지의 가로, 세로
			(int)m_fBlockSize,
			hMemDC,						// 복사할 이미지 DC
			206,							// 비트맵 출력 시작 좌표(Left, top)
			3,
			SRCCOPY);
		break;
	case BLOCK_GUT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile_Gut");

		BitBlt(hDC,						// 복사 받을 DC
			0,	// 복사 받을 위치 좌표 X, Y	
			0,
			(int)m_fBlockSize,			// 복사 받을 이미지의 가로, 세로
			(int)m_fBlockSize,
			hMemDC,						// 복사할 이미지 DC
			2,							// 비트맵 출력 시작 좌표(Left, top)
			3,
			SRCCOPY);
		break;
	}


	//박스 인스턴스 출력
	//for (auto& pLine : m_ObjList)
		//pLine->Render(hDC);

	//선 그리기
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
	//HEAD와TAIL의 거리 / 박스 크기 = 박스 갯수
	int iBoxCount = int(fDist / m_fBlockSize);

	//HEAD부터 TAIL까지 박스들 출력
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
	HANDLE		hFile = CreateFile(L"../Data/Block_Silver.dat", // 파일 경로와 이름을 명시
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

	//각 각 오브젝트 리스트 마다 객체의 CObj를 저장한다.
	for (auto pObj : CObjMgr::Get_Instance()->Get_ObjList()[OBJ_BLOCK])
	{
		WriteFile(hFile,
			pObj,
			sizeof(CBlock),
			&dwByte, nullptr);
	}


	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CBlockMgr::Load_Block()
{
	Release();

	HANDLE		hFile = CreateFile(L"../Data/Block_Silver.dat", // 파일 경로와 이름을 명시
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
	CBlock  Block;
	//ReadFile해서 어떤 오브젝트인지 판단한다음(CObj에 타입 멤버변수를 둬서 판단해야 겠다.)
	//해당 리스트에 넣어줘야한다.
	while (true)
	{
		//블럭 하나를 가져온다.
		bool bResult = ReadFile(hFile,
			&Block,
			sizeof(CBlock),
			&dwByte, nullptr);
		//읽어온게 없으면 끝
		if (0 == dwByte)
			break;

		CObj* pNewObj(nullptr);
		//블럭 타입 확인
		switch (Block.Get_BlockType())
		{
		case BLOCK_ICE:
			//Info값 대로 블럭 생성
			pNewObj = CAbstractFactory<CBlock_Ice>::Create(OBJ_BLOCK, Block.Get_Info());
			//해당 리스트에 오브젝트 추가
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_FIRE:
			//Info값 대로 블럭 생성
			pNewObj = CAbstractFactory<CBlock_Fire>::Create(OBJ_BLOCK, Block.Get_Info());
			//해당 리스트에 오브젝트 추가
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_ELEC:
			//Info값 대로 블럭 생성
			pNewObj = CAbstractFactory<CBlock_Elec>::Create(OBJ_BLOCK, Block.Get_Info());
			//해당 리스트에 오브젝트 추가
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_CUT:
			//Info값 대로 블럭 생성
			pNewObj = CAbstractFactory<CBlock_Cut>::Create(OBJ_BLOCK, Block.Get_Info());
			//해당 리스트에 오브젝트 추가
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		case BLOCK_GUT:
			//Info값 대로 블럭 생성
			pNewObj = CAbstractFactory<CBlock_Gut>::Create(OBJ_BLOCK, Block.Get_Info());
			//해당 리스트에 오브젝트 추가
			CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, pNewObj);
			break;
		}
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
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
