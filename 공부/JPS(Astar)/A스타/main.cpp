// A스타.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "Map.h"
#include "PathFinder.h"
#include <time.h>
#ifdef _DEBUG            // 메모리 누수 체크 매크로

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // 메모리 누수 체크 명령어
    //_CrtSetBreakAlloc(316); //번호로 누수 위치 확인 가능

    //col = 가로, lines = 세로
    system("mode con:cols=100 lines=50");

    SetConsoleTitle(L"Text RPG");

    const int iMapSize(40);

    CMap Map(iMapSize, iMapSize);
    Map.Initialize();

    /*srand((time(NULL)));
    for (int i = 0; i < iMapSize; ++i)
    {
        Map.Change_Tile(POS(rand() % iMapSize, rand() % iMapSize), WALL);
    }*/
    
    for (int i = 0; i < 8; ++i)
    {
        Map.Change_Tile(POS(4, i), WALL);
    }

    for (int i = 1; i < 8; ++i)
    {
        Map.Change_Tile(POS(18, i), WALL);
    }

    for (int i = 5; i < 19; ++i)
    {
        Map.Change_Tile(POS(6, i), WALL);
    }

    for (int i = 10; i < 20; i += 2)
    {
        Map.Change_Tile(POS(10, i), WALL);
    }

    for (int i = 2; i < 19; ++i)
    {
        Map.Change_Tile(POS(15, i), WALL);
    }
 
    for (int i = 5; i < 19; ++i)
    {
        Map.Change_Tile(POS(i, 7), WALL);
    }

    Map.Change_Tile(POS(0, 0), START);
    //Map.Change_Tile(POS(iMapSize - 1, iMapSize - 1), END);
    Map.Change_Tile(POS(5, 6), END);

    CPathFinder PathFinder;
    PathFinder.Initialize(Map);

    ULONGLONG		dwTime = GetTickCount64();

    Map.Set_PrintPathOnly(true);
    //PathFinder.Start_A_Star();
    PathFinder.Start_JPS();

    //Map.Render_Test();

    cout << GetTickCount64() - dwTime << endl;

    system("pause");

}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
