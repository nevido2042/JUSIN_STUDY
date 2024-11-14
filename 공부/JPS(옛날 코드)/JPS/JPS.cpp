#include "framework.h"
#include "JPS.h"
#include<list>
#include <math.h>
#include<stdio.h>

#include"TileManager.h"

using namespace std;

//-----------------------------------//

//bool isStarted = false;

enum Direction
{
	OO = 0,
	UU,
	RU,
	RR,
	RD,
	DD,
	LD,
	LL,
	LU
};

struct Node
{
	int x;
	int y;

	int g;
	int h;
	int f;

	Node* parent;

	Direction dir;
};


list<Node*> openList;
list<Node*> closeList;
list<Node*> bresenhamList;

#define COST 10


//bool isLine = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//wprintf_s(L"openListsize: %d, closeListsize: %d\n", openList.size(), closeList.size());

	CTileManager TileManager;
	TileManager.Initialize();

	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_CONTROL)
		{
			InvalidateRect(hWnd, NULL, false);

			TileManager.Set_OldPos(TileManager.Get_CurrentPos());

			TileManager.Set_OldTile(TileManager.Get_OldPos());

		}
	}
	break;
	case WM_RBUTTONDOWN:
	{
		//우클릭
		POS ClickPos(LOWORD(lParam) / GRID_SIZE, HIWORD(lParam) / GRID_SIZE);
		//const int iX = LOWORD(lParam) / GRID_SIZE;
		//const int iY = HIWORD(lParam) / GRID_SIZE;

		if (ClickPos.iX < 0 && ClickPos.iX >= GRID_WIDTH && ClickPos.iY < 0 && ClickPos.iY >= GRID_HEIGHT)
		{
			break;
		}

		//시작타일 없으면 -> 시작타일 만든다
		if (TileManager.Get_StartPos().iX == -1)
		{
			TileManager.Set_Tile(ClickPos.iX, ClickPos.iY, START);
			//g_Tile[ClickPos.iY][ClickPos.iX] = START;
			TileManager.Set_StartPos(ClickPos);
			//g_Start(ClickPos.iX, ClickPos.iY);
			
		}
		//시작타일 있는데 끝타일이 없으면 -> 끝타일을 만든다.
		else if (TileManager.Get_StartPos().iX != -1 && TileManager.Get_EndPos().iX == -1)
		{
			TileManager.Set_Tile(ClickPos.iX, ClickPos.iY, END);
			//g_Tile[ClickPos.iY][ClickPos.iX] = END;
			TileManager.Set_EndPos(ClickPos);
			//g_end(ClickPos.iX, ClickPos.iY);
		}
		//시작타일, 끝타일 모두 있으면 -> 시작타일의 위치를 바꾸고 끝타일을 제거
		else if(TileManager.Get_StartPos().iX != -1 && TileManager.Get_EndPos().iX != -1)
		{
			TileManager.Set_Tile(TileManager.Get_EndPos().iX, TileManager.Get_EndPos().iY, NONE);
			//g_Tile[g_end.iY][g_end.iX] = NONE;
			
			TileManager.Set_EndPos();
			//g_end(-1, -1);

			TileManager.Set_Tile(TileManager.Get_StartPos().iX, TileManager.Get_StartPos().iY, NONE);
			//g_Tile[g_Start.iY][g_Start.iX] = NONE;
			//g_Start(ClickPos.iX, ClickPos.iY);
			TileManager.Set_StartPos(ClickPos);

			//g_Tile[ClickPos.iY][ClickPos.iX] = START;
			TileManager.Set_Tile(ClickPos.iX, ClickPos.iY, START);
			
		}
		InvalidateRect(hWnd, NULL, false);
	}
	break;
	case WM_LBUTTONDOWN:
		TileManager.Set_IsDrag(true);
		//g_bDrag = true;
		{
			POS ClickPos(LOWORD(lParam) / GRID_SIZE, HIWORD(lParam) / GRID_SIZE);
			/*int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			int iTileX = xPos / GRID_SIZE;
			int iTileY = yPos / GRID_SIZE;*/
			//첫 선택 타일이 장애물이면 지우기모드 아니면 장애물 넣기 모드
			if (TileManager.Get_Tile(ClickPos.iX, ClickPos.iY) == OBSTACLE)
			{
				TileManager.Set_IsErase(true);
				//g_bErase = true;
			}
			else
			{
				TileManager.Set_IsErase(false);
				//g_bErase = false;
			}
		}
		break;

	case WM_LBUTTONUP:
		TileManager.Set_IsDrag(false);
		//g_bDrag = false;
		break;

	case WM_MOUSEMOVE:
	{
		//마우스 현재위치 갱신
		POS MousePos(LOWORD(lParam), HIWORD(lParam));
		TileManager.Set_CurrentPos(MousePos);

		if (TileManager.Get_IsDrag())
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);

			int iTileX = xPos / GRID_SIZE;
			int iTileY = yPos / GRID_SIZE;

			if (iTileX<0 || iTileX>GRID_WIDTH || iTileY<0 || iTileY>GRID_HEIGHT)//다른 전역변수 건들이는 것 방지
				return 0;

			//g_Tile[iTileY][iTileX] = !g_bErase;
			TileManager.Set_Tile(iTileX, iTileY, !TileManager.Get_IsErase());
			InvalidateRect(hWnd, NULL, false);
		}
		//...기존 코드들

		//InvalidateRect(hWnd, NULL, false);
		//마우스 드래그로 데이터가 변경되어 갱신을 요청 할 시 마지막 Erase플래그를 false로 하여 화면
		//깜빡임을 없앤다. WM_PAINT에서는 윈도우 전체를 덮어 쓰기 때문에 지우지 않아도 된다.

	}
	break;

	case WM_DESTROY:
		SelectObject(TileManager.Get_DC(), TileManager.Get_DCBitmap_Old());
		DeleteObject(TileManager.Get_DC());
		DeleteObject(TileManager.Get_DCBitmap());

		PostQuitMessage(0);
		break;

	case WM_CREATE:
		//윈도우 생성 시 현 윈도우 크기와 동일한 메모리DC생성
	{
		HDC hdc = GetDC(hWnd);
		GetClientRect(hWnd, &TileManager.Get_DCRect());
		TileManager.Set_DCBitmap(CreateCompatibleBitmap(hdc, TileManager.Get_DCRect().right, TileManager.Get_DCRect().bottom));
		TileManager.Set_DC(CreateCompatibleDC(hdc));
		ReleaseDC(hWnd, hdc);
		TileManager.Set_DCBitmap_Old((HBITMAP)SelectObject(TileManager.Get_DC(), TileManager.Get_DCBitmap()));
	}

	break;

	case WM_PAINT:
	{
		//메모리 DC를 클리어하고
		PatBlt(TileManager.Get_DC(), 0, 0, TileManager.Get_DCRect().right, TileManager.Get_DCRect().bottom, WHITENESS);

		//RenderObstacle, RenderGrid를 메모리 DC에 출력
		TileManager.RenderTile();
		TileManager.RenderGrid();

		//메모리 DC에 랜더링이 끝나면, 메모리 DC -> 윈도우 DC로의 출력
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, TileManager.Get_DCRect().right, TileManager.Get_DCRect().bottom, TileManager.Get_DC(), 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_SIZE:
	{
		SelectObject(TileManager.Get_DC(), TileManager.Get_DCBitmap_Old());
		DeleteObject(TileManager.Get_DC());
		DeleteObject(TileManager.Get_DCBitmap());

		HDC hdc = GetDC(hWnd);

		GetClientRect(hWnd, &TileManager.Get_DCRect());
		TileManager.Set_DCBitmap(CreateCompatibleBitmap(hdc, TileManager.Get_DCRect().right, TileManager.Get_DCRect().bottom));
		TileManager.Set_DC(CreateCompatibleDC(hdc));
		ReleaseDC(hWnd, hdc);

		TileManager.Set_DCBitmap_Old((HBITMAP)SelectObject(TileManager.Get_DC(), TileManager.Get_DCBitmap()));
	}
	break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	//도스 창 띄움
	AllocConsole();
	FILE* file = nullptr;
	freopen_s(&file, "CONOUT$", "wt", stdout);

	WNDCLASS wc;
	wchar_t my_class_name[] = L"MyClass";
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, L"JPS",
		WS_OVERLAPPEDWINDOW, 100, 90, 400, 350, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;

	// 도스 창 해제할때
	FreeConsole();
}
