// RimWorld.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "RimWorld.h"
#include "MainGame.h"
#include "TimeMgr.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;
ULONG_PTR gdiplusToken;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);   // 창의 외형적인 스타일을 지정하는 옵션 함수
BOOL                InitInstance(HINSTANCE, int);           // 창 초기화 함수
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);    // 윈도우 메시지 처리기 함수
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 현재 프로그램의 고유 식별 번호
                     _In_opt_ HINSTANCE hPrevInstance,  // 전에 실행되었던 인스턴스의 핸들(없을 경우 NULL)
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)           // 창의 스타일(모양 / 최소화 또는 최대한 모양)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //ULONG_PTR GdiplusToken;
    //GdiplusStartupInput GdiplusStartupInput;//옵션 설정 변수
    //if (GdiplusStartup(&GdiplusToken, &GdiplusStartupInput, NULL) != Ok) return 0;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
     // GDI+ 초기화
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RIMWORLD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RIMWORLD));

    MSG msg;
    msg.message = WM_NULL;

    CMainGame       MainGame;
    MainGame.Initialize();
    CTimeMgr::Get_Instance()->Initialize();

    ULONGLONG ullTime = GetTickCount64();

    // 기본 메시지 루프입니다:
    while (true)
    {
        // PeekMessage : 시스템 메세지 큐로부터 메세지를 읽어오면 TRUE, 읽어올 메세지가 없을 경우 FALSE

        // PM_REMOVE       : 메세지를 읽어옴과 동시에 메세지 제거
        // PM_NOREMOVE     : 메세지 큐에 메세지가 있는지 파악, 메세지가 있을 경우, GetMessage를 호출하여 true처리

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            // 메뉴 기능의 단축키가 제대로 작동하도록 검사하는 함수
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                // 키보드 메세지를 가공하여 프로그램에서 쉽게 사용할 수 있도록 하는 함수
                TranslateMessage(&msg);

                // 시스템 메세지 큐에서 꺼낸 메세지를 프로그램의 메세지 처리기에게 전달하는 함수
                DispatchMessage(&msg);
            }
        }

        else
        {
            if (ullTime + 10 < GetTickCount64())
            {
                CTimeMgr::Get_Instance()->Update();
                MainGame.Update();
                MainGame.Late_Update();
                MainGame.Render();
            
                ullTime = GetTickCount64();
            }

            /*CTimeMgr::Get_Instance()->Update();
            MainGame.Update();
            MainGame.Late_Update();
            MainGame.Render();*/
        }      
    }
    GdiplusShutdown(gdiplusToken);
    //GdiplusShutdown(g_GdiplusToken);//Gdiplus 정리
    CTimeMgr::Destroy_Instance();

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // WNDCLASSEXW : 창의 여러 특성을 정의하기 위한 구조체
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    // 자기 자신의 사이즈를 저장

    // 초기화 되는 값은 윈도우 창의 수직, 수평의 크기가 변할 경우 다시 그리겠다는 의미
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // CS_HREDRAW : 가로를 다시 그림
    // CS_VREDRAW : 세로를 다시 그림

    // 저장해둔 함수가 호출되어 메세지를 처리
    wcex.lpfnWndProc    = WndProc;

    // 윈도우가 특수한 목적으로 사용하는 여분의 공간(일종의 예약 영역, 사용하지 않을 경우 0)
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

    // 이 윈도우를 사용하는 프로그램의 번호를 설정
    wcex.hInstance      = hInstance;

    // 윈도우가 사용할 아이콘 지정
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RIMWORLD));

    // 마우스 커서 지정
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    // 리소스에서 커서를 로드
    wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CUSTOM_CURSOR));

    
    // 배경 색을 설정
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);

    // 창의 메뉴바
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_RIMWORLD);
   
    // 실행 파일의 이름(프로젝트 이름과 일치시켜 작성)
    wcex.lpszClassName  = szWindowClass;

    // 윈도우 상단에 있는 아이콘 모양
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
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // CreateWindowW : 등록해 놓은 정보들을 토대로 윈도우를 생성

   RECT rc{ 0, 0, WINCX, WINCY };

   // rc = rc + 기본 창 스타일을 고려한 크기 + 메뉴 바 크기 고려 여부
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass,         // 정의한 윈도우 클래스의 전달할 클래스 이름
                             szTitle,               // 창 타이틀 문자열
                             WS_OVERLAPPEDWINDOW,   // 만들고자 하는 윈도우의 형태(스타일)지정, 기본 값으로 흔히 접하는 모양
                             CW_USEDEFAULT, 0,      // 창을 생성하는 X,Y 좌표
                             rc.right - rc.left,
                             rc.bottom - rc.top,    // 생성할 창의 가로, 세로 사이즈
                             nullptr,               // 부모 윈도우의 핸들을 지정, 없으면 null
                             nullptr,               // 윈도우에서 사용할 메뉴의 핸들
                             hInstance,             // 윈도우를 만드는 주체, 프로그램의 핸들 지정
                             nullptr);              // 운영체제가 특수한 목적으로 사용

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// 함수 호출 규약  : 함수의 실행이 종료되면 할당되었던 스택 메모리를 반환해야하는데 함수를 호출한 호출원(caller)에서 정리할 것인가
// 아니면 호출 받은 피호출자(callee)에서 할 것인가에 대한 약속 또는 규약

// __cdecl : c / c++ 기본 호출 규약, 호출원이 stack을 정리, 대표적인 예로 printf 함수를 생각하면 된다(가변인자 함수)
// __stdcall : win api 기본 호출 규약, 피호출자가 stack을 정리 (고정인자 함수)
// __fastcall : 함수 호출을 빠르게 처리하기 위한 호출 규약
// __thiscall : this 포인터를 매개 변수로 전달 받았을 경우 사용되는 규약


// 라이브러리 : 바이너리로 번역(컴파일)된 함수들을 모아놓은 파일

// - 코드 재사용을 위한 초창기 방법 중 하나이며 다른 프로그램에서 사용할 수 있도록 운영체제나 소프트웨어 개발자에 의해 제공
// - 라이브러리의 대다수 함수들은 함수의 정의 및 선언을 필요로하기 때문에 링크되기 전 전처리기에 의해 해당 헤더파일을 원시 프로그램에 포함
// - 라이브러리들은 사용자의 프로그램과 링크되어 실행이 가능한 완전한 프로그램을 이루게 됨

// 윈도우 구성 라이브러리

// 1. KERNEL : 메모리를 관리하고 프로그램을 실행
// 2. USER : 유저 인터페이스와 윈도우(창)을 관리
// 3. GDI : 화면 처리와 그래픽을 담당

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
   // case WM_CREATE:
   //
   //     SetTimer(hWnd, 0, 0, 0);
   //
   //     // 타이머 설치 함수
   //     // 1인자 : 윈도우 핸들
   //     // 2인자 : 타이머의 ID
   //     // 3인자 : 타이머 주기, 기본값으로 1 / 1000초
   //     // 4인자 : NULL일 경우 설정한 주기대로 WM_TIMER 메세지를 발생시킴
   //     break;
   //
   // case WM_TIMER:
   //     InvalidateRect(hWnd, 0, TRUE);
   //
   //     // InvalidateRect : 윈도우 화면 갱신 함수, 이 함수 호출 시, WM_PAINT 메세지 발생
   //
   //     // 1인자 : 갱신할 윈도우 핸들
   //     // 2인자 : 윈도우에서 갱신할 범위 지정, RECT 구조체 주소를 넣어주며 NULL일 경우 윈도우 전체 영역에 적용
   //     // 3인자 : TRUE일 경우 그려져 있지 않는 부분도 갱신
   //     //        FALSE 일 경우 그린 부분만 갱신
   //     break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(g_hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;

        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return OBJ_NOEVENT;
}

// 숙제 1. 그림을 그려와라.
// 숙제 2. 세 방향으로 총알을 쏘도록 만들어라.

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


