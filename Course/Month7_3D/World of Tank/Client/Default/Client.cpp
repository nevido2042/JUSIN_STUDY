﻿// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Client.h"

#include "MainApp.h"
#include "GameInstance.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
//HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    g_iWinSizeX = 1280;
    g_iWinSizeY = 720;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    CMainApp* pMainApp = { nullptr };
    CGameInstance* pGameInstance = CGameInstance::Get_Instance();
    if (nullptr == pGameInstance)
        return FALSE;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    pMainApp = CMainApp::Create();
    if (nullptr == pMainApp)
        return FALSE;

    if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Default"))))
        return FALSE;

    if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_60"))))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    _float      fTimeAcc = {};

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        pGameInstance->Update_Timer(TEXT("Timer_Default"));

        fTimeAcc += pGameInstance->Get_TimeDelta(TEXT("Timer_Default"));        

        if (fTimeAcc >= 1.f / 60.f)
        {
            pGameInstance->Update_Timer(TEXT("Timer_60"));

            pMainApp->Update(pGameInstance->Get_TimeDelta(TEXT("Timer_60")));
            pMainApp->Render();

            fTimeAcc = 0.f;
        }
        
    } 
	Safe_Release(pGameInstance);

    if (0 != Safe_Release(pMainApp))
        MSG_BOX("Failed to Release : CMainApp");

#pragma region ReportLiveObjects

    //typedef HRESULT(WINAPI* LPDXGIGETDEBUGINTERFACE)(REFIID, void**);
    //HMODULE dxgiDebugDll = LoadLibraryW(L"dxgidebug.dll");
    //if (!dxgiDebugDll)
    //{
    //    OutputDebugStringW(L"[ReportLiveObjects] dxgidebug.dll 로드 실패\n");
    //}

    //auto GetDebugInterface = (LPDXGIGETDEBUGINTERFACE)GetProcAddress(dxgiDebugDll, "DXGIGetDebugInterface");
    //if (!GetDebugInterface)
    //{
    //    OutputDebugStringW(L"[ReportLiveObjects] GetProcAddress 실패\n");
    //}
    //if (!GetDebugInterface)
    //{
    //    FreeLibrary(dxgiDebugDll);
    //    //return;
    //}

    //IDXGIDebug* pDebug = nullptr;
    //if (SUCCEEDED(GetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDebug)))
    //{
    //    if (!pDebug)
    //    {
    //        OutputDebugStringW(L"[ReportLiveObjects] IDXGIDebug 인터페이스 획득 실패\n");
    //    }

    //    DXGI_DEBUG_RLO_FLAGS flags = static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL);
    //    pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, flags);
    //    pDebug->Release();
    //}

    //FreeLibrary(dxgiDebugDll);
#pragma endregion

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT     rcWindowed = { 0, 0, static_cast<LONG>(g_iWinSizeX), static_cast<LONG>(g_iWinSizeY) };

   AdjustWindowRect(&rcWindowed, WS_OVERLAPPEDWINDOW, true);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rcWindowed.right - rcWindowed.left, rcWindowed.bottom - rcWindowed.top,  nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED) // 최소화 상태가 아닐 때만 처리
        { 
            g_iWinSizeX = LOWORD(lParam); // 새 너비
            g_iWinSizeY = HIWORD(lParam); // 새 높이
            g_bWindowResizeRequired = true;    // 리사이즈 필요 플래그 설정
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
