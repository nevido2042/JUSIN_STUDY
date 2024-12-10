#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
	:m_hBitmap(nullptr), m_hMemDC(nullptr), m_hOldmap(nullptr)
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,		// ���α׷� �ν��Ͻ� �ڵ�, ���Ϸκ��� ���� �о�� ���̱� ������ NULL
									pFilePath,	// ������ ���
									IMAGE_BITMAP,	// � Ÿ���� �����͸� �о�� ���ΰ�
									0, 0,			// �̹��� ����, ���� ������, ������ ���� �о���� ���̾ ������ �Ѱ��� �ʿ� ����
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);	// LR_LOADFROMFILE : ���Ϸκ��� �̹����� �о����
																			// LR_CREATEDIBSECTION : �о�� ������ DIB ���·� ��ȯ

	// SelectObject : GDI ������Ʈ�� �����ϱ� ���� ������ ������ �ִ� ������Ʈ�� ��ȯ
	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);

}
