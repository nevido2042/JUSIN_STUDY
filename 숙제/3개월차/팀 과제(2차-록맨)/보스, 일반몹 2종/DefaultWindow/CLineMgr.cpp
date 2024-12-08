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

bool CLineMgr::Collision_Line(float _fX, float* pY, float _fY)
{
    if (m_Linelist.empty())
        return false;

    CLine* pTargetLine = nullptr;
    float minYDistance = FLT_MAX;

    for (auto& pLine : m_Linelist)
    {
        if (_fX >= pLine->Get_Info().tLPoint.fX &&
            _fX < pLine->Get_Info().tRPoint.fX)
        {
            // ���� ���� ��ǥ ���
            float x1 = pLine->Get_Info().tLPoint.fX;
            float y1 = pLine->Get_Info().tLPoint.fY;
            float x2 = pLine->Get_Info().tRPoint.fX;
            float y2 = pLine->Get_Info().tRPoint.fY;

            // �ش� X������ Y ��ǥ ���
            float calculatedY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

            // Y ��ǥ���� �Ÿ� ���
            float yDistance = fabs(calculatedY - _fY);

            // Y �Ÿ��� �ּ��� �� ����
            if (yDistance < minYDistance)
            {
                minYDistance = yDistance;
                pTargetLine = pLine;
                *pY = calculatedY; // ���� Y ��ǥ ����
            }
        }
    }

    if (!pTargetLine)
        return false;

    return true;
}

void CLineMgr::Initialize()
{
    //if (g_CurrentStage == 0)
        //return;  // ���۽��������ϰ�� 
    // �ε���� ������ �ؾߵ�
	//Load_Line();
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

    // ���������� ���� ��� ����
    LPCWSTR filePath = nullptr;

    //switch (g_CurrentStage) // ���� �������� ����
    //{
    //case STAGE_HONG: // 1�� ȫ����
    //    filePath = L"../Data/Line_Hong.dat";
    //    break;
    //case STAGE_SILVER:// 2�� ������
    //    filePath = L"../Data/Line_Silver.dat";
    //    break;
    //case STAGE_HERO://3�� �ѿ���
    //    filePath = L"../Data/Line_Hero.dat";
    //    break;
    //case STAGE_CHAE://4�� ��ä��
    //    filePath = L"../Data/Line_Chae.dat";
    //    break;
    //case STAGE_MOO://5�� �ӹ���
    //    filePath = L"../Data/Line_Moo.dat";
    //    break;
    //default:
    //    MessageBox(g_hWnd, _T("Invalid Stage"), L"Error", MB_OK);
    //    return;
    //}

    HANDLE hFile = CreateFile(filePath,        // ���� ��ο� �̸�
        GENERIC_READ,    // �б� ���
        NULL,            // ���� ���
        NULL,            // ���� �Ӽ�
        OPEN_EXISTING,   // ���� ���� ����
        FILE_ATTRIBUTE_NORMAL,
        NULL);           // ���ø� ����

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

    MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}

void CLineMgr::Load_Line(LPCWSTR _FilePath)
{
    Release();

    HANDLE hFile = CreateFile(_FilePath,        // ���� ��ο� �̸�
        GENERIC_READ,    // �б� ���
        NULL,            // ���� ���
        NULL,            // ���� �Ӽ�
        OPEN_EXISTING,   // ���� ���� ����
        FILE_ATTRIBUTE_NORMAL,
        NULL);           // ���ø� ����

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

    MessageBox(g_hWnd, _T("Load �Ϸ�"), L"����", MB_OK);
}
