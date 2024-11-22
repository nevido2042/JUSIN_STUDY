// 그래프.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "ALGraph.h"
#include "ALGraph_Search.h"
#include "ALGraph_Kruskal.h"

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
    
    /*CALGraph Graph;
    Graph.Initialize(5);

    Graph.AddEdge(A, B);
    Graph.AddEdge(A, D);
    Graph.AddEdge(B, C);
    Graph.AddEdge(C, D);
    Graph.AddEdge(D, E);
    Graph.AddEdge(E, A);

    Graph.Show_GraphEdgeInfo();*/

    //CALGraph_Search Graph;
    //Graph.Initialize(7);

    //Graph.AddEdge(A, B);
    //Graph.AddEdge(A, D);
    //Graph.AddEdge(B, C);
    //Graph.AddEdge(D, C);
    //Graph.AddEdge(D, E);
    //Graph.AddEdge(E, F);
    //Graph.AddEdge(E, G);

    //Graph.Show_GraphEdgeInfo();

    ///*Graph.Show_DFS_GraphVertex(A);
    //cout << endl;
    //Graph.Show_DFS_GraphVertex(C);
    //cout << endl;
    //Graph.Show_DFS_GraphVertex(E);
    //cout << endl;
    //Graph.Show_DFS_GraphVertex(G);
    //cout << endl;*/

    //Graph.Show_BFS_GraphVertex(A);
    //cout << endl;
    //Graph.Show_BFS_GraphVertex(C);
    //cout << endl;
    //Graph.Show_BFS_GraphVertex(E);
    //cout << endl;
    //Graph.Show_BFS_GraphVertex(G);
    //cout << endl;

    CALGraph_Kruskal Graph;
    Graph.Initialize(6);

    Graph.AddEdge(A, B, 9);
    Graph.AddEdge(B, C, 2);
    Graph.AddEdge(A, C, 12);
    Graph.AddEdge(A, D, 8);
    Graph.AddEdge(D, C, 6);
    Graph.AddEdge(A, F, 11);
    Graph.AddEdge(F, D, 4);
    Graph.AddEdge(D, E, 3);
    Graph.AddEdge(E, C, 7);
    Graph.AddEdge(F, E, 13);

    Graph.Show_GraphEdgeInfo();
    Graph.Construct_KruskalMST();
    cout << endl;
    Graph.Show_GraphEdgeInfo();
    Graph.Show_GraphEdgeWeightInfo();
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
