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
		if (_fX >= pLine->Get_Info().tLPoint.fX &&
			_fX < pLine->Get_Info().tRPoint.fX)
		{
			pTargetLine = pLine;
		}
	}

	if (!pTargetLine)
		return false;

	float	x1 = pTargetLine->Get_Info().tLPoint.fX;
	float	y1 = pTargetLine->Get_Info().tLPoint.fY;
	float	x2 = pTargetLine->Get_Info().tRPoint.fX;
	float	y2 = pTargetLine->Get_Info().tRPoint.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

	return true;
}

void CLineMgr::Initialize()
{
    if (g_CurrentStage == 0)
        return;  // 시작스테이지일경우 
    // 로드라인 못들어가게 해야됨
	Load_Line();
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


void CLineMgr::Load_Line()
{
    Release();

    // 스테이지별 파일 경로 설정
    LPCWSTR filePath = nullptr;

    switch (g_CurrentStage) // 현재 스테이지 숫자
    {
    case STAGE_HONG: // 1번 홍동완
        filePath = L"../Data/Line_Hong.dat";
        break;
    case STAGE_SILVER:// 2번 최은성
        filePath = L"../Data/Line_Silver.dat";
        break;
    case STAGE_HERO://3번 한영웅
        filePath = L"../Data/Line_Hero.dat";
        break;
    case STAGE_CHAE://4번 박채영
        filePath = L"../Data/Line_Chae.dat";
        break;
    case STAGE_MOO://5번 임무결
        filePath = L"../Data/Line_Moo.dat";
        break;
    default:
        MessageBox(g_hWnd, _T("Invalid Stage"), L"Error", MB_OK);
        return;
    }

    HANDLE hFile = CreateFile(filePath,        // 파일 경로와 이름
        GENERIC_READ,    // 읽기 모드
        NULL,            // 공유 방식
        NULL,            // 보안 속성
        OPEN_EXISTING,   // 기존 파일 열기
        FILE_ATTRIBUTE_NORMAL,
        NULL);           // 템플릿 파일

    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
        return;
    }

    DWORD dwByte = 0;
    LINE tLine{};

    while (true)
    {
        bool Garbage;
        Garbage = ReadFile(hFile, &tLine, sizeof(LINE), &dwByte, nullptr);

        if (dwByte == 0)
            break;

        m_Linelist.push_back(new CLine(tLine));
    }

    CloseHandle(hFile);

    MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}
