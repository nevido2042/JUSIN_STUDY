﻿
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
	: m_pDevice(CDevice::Get_Instance())
	, m_pTerrain(nullptr)

{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// AfxGetMainWnd : 현재 메인 윈도우의 값을 반환하는 전역함수

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rcWnd{};

	// GetWindowRect : 현재 윈도우(창)의 rect 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수 대로 Rect 정보를 기록하는 함수

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	// rcWnd.right : 창의 가로 사이즈
	// rcWnd.bottom : 창의 세로 사이즈

	RECT rcMainView{};

	// GetClientRect : 현재 View창의 정보를 Rect 형식으로 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수대로 윈도우 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치지정 옵션)
	
	pMainFrm->SetWindowPos(nullptr,		// 순서 변경하지 않음
							0, 
							0,
							int(WINCX + fRowFrm), 
							int(WINCY + fColFrm), 
							SWP_NOZORDER); // 현재 순서를 유지함


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Init_Device Create Failed");
		return;
	}
	
	m_pTerrain = new CTerrain;
	if (FAILED(m_pTerrain->Initialize()))
	{
		AfxMessageBox(L"m_pTerrain Create Failed");
		return;
	}

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// point.x, point.y

	CView::OnLButtonDown(nFlags, point);

	m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x), float(point.y), 0.f), 0);

	// Invalidate : 호출 시 윈도우에 WM_PAINT와 WM_ERASEBKGND 메세지를 발생시킴
	// WM_PAINT 메세지 발생 시, OnDraw함수가 다시 호출

	// false : WM_PAINT 메세지만 발생
	// true : WM_PAINT, WM_ERASEBKGND 둘 다 메세지 발생

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x), float(point.y), 0.f), 0);
		Invalidate(FALSE);
	}
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pDevice->Render_Begin();

	m_pTerrain->Render();

	m_pDevice->Render_End();


}

void CToolView::OnDestroy()
{
	CView::OnDestroy();

	Safe_Delete(m_pTerrain);

	CTextureMgr::Destroy_Instance();
	m_pDevice->Destroy_Instance();

}


#pragma region 생략
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion








