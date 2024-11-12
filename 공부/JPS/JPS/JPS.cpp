#include "framework.h"
#include "JPS.h"
//#include "list.h"
#include<list>
#include <math.h>
#include<stdio.h>
//#include <time.h>

using namespace std;

#define GRID_SIZE 16
#define GRID_WIDTH 100
#define GRID_HEIGHT 50

HBRUSH g_hTileBrush;
HPEN g_hGridPen;
char g_Tile[GRID_HEIGHT][GRID_WIDTH]; //1 장애물 있음 / 0 장애물 없음

bool g_bErase = false;
bool g_bDrag = false;

HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;
HDC g_hMemDC;
RECT g_MemDCRect;

//-----------------------------------//
enum TileType
{
	NONE = 0,
	OBSTACLE,
	START,
	END,
	CHECK,
	NODE,
	LINE
};

typedef struct tagPosition
{
	int iX;
	int iY;
	tagPosition(int _iX = -1, int _iY = -1) : iX(_iX), iY(_iY) {};
	void operator()(int _iX = -1, int _iY = -1)
	{
		iX = _iX;
		iY = _iY;
	}
}POS;

POS g_Start;
POS g_end;

HBRUSH g_hStartBrush;
HBRUSH g_hendBrush;
HBRUSH g_hCheckBrush;
HBRUSH g_hNodeBrush;
HPEN g_hParentPen_Open;
HPEN g_hParentPen_Close;
HPEN g_hBresenhamPen;

bool isStarted = false;

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


bool isLine = false;

POS g_OldPos;
POS g_CurrentPos;
POS g_OldTile;

HBRUSH g_hLineBrush;
//int old_iTileX;
//int old_iTileY;
//-----------------------------------//

void RenderGrid(HDC hdc)
{
	int iX = 0;
	int iY = 0;
	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hGridPen);
	//그리드의 마지막 선을 추가로 그리기 위해 <=의 반복 조건
	for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
	{
		MoveToEx(hdc, iX, 0, NULL);
		LineTo(hdc, iX, GRID_HEIGHT * GRID_SIZE);
		iX += GRID_SIZE;
	}
	for (int iCntH = 0; iCntH <= GRID_HEIGHT; iCntH++)
	{
		MoveToEx(hdc, 0, iY, NULL);
		LineTo(hdc, GRID_WIDTH * GRID_SIZE, iY);
		iY += GRID_SIZE;
	}
	SelectObject(hdc, hOldPen);
}

void RenderParent(HDC hdc)
{
	HPEN hOldPen;
	list<Node*>::iterator iter;
	if (openList.size() > 0)
	{
		//오픈리스트 클로즈리스트에 있는 노드들은 순환하며
		//자신의 위치와, 부모의 위치를 잇는다
		hOldPen = (HPEN)SelectObject(hdc, g_hParentPen_Open);

		for (iter = openList.begin(); iter != openList.end(); iter++)
		{
			int myX = GRID_SIZE / 2 + (*iter)->x * GRID_SIZE;
			int myY = GRID_SIZE / 2 + (*iter)->y * GRID_SIZE;
			int parentX = GRID_SIZE / 2 + (*iter)->parent->x * GRID_SIZE;
			int parentY = GRID_SIZE / 2 + (*iter)->parent->y * GRID_SIZE;

			MoveToEx(hdc, myX, myY, NULL);
			LineTo(hdc, parentX, parentY);
		}

		SelectObject(hdc, hOldPen);
	}

	if (closeList.size() > 0)
	{
		hOldPen = (HPEN)SelectObject(hdc, g_hParentPen_Close);

		for (iter = closeList.begin(); iter != closeList.end(); iter++)
		{
			if ((*iter)->parent == nullptr)
				continue;

			int myX = GRID_SIZE / 2 + (*iter)->x * GRID_SIZE;
			int myY = GRID_SIZE / 2 + (*iter)->y * GRID_SIZE;
			int parentX = GRID_SIZE / 2 + (*iter)->parent->x * GRID_SIZE;
			int parentY = GRID_SIZE / 2 + (*iter)->parent->y * GRID_SIZE;

			MoveToEx(hdc, myX, myY, NULL);
			LineTo(hdc, parentX, parentY);
		}

		SelectObject(hdc, hOldPen);
	}
}

void RenderLine(HDC hdc)
{
	HPEN hOldPen = (HPEN)SelectObject(hdc, g_hParentPen_Close);

	MoveToEx(hdc, g_OldPos.iX, g_OldPos.iY, NULL);
	LineTo(hdc, g_CurrentPos.iX, g_CurrentPos.iY);

	SelectObject(hdc, hOldPen);
}

void RenderBresenhamLine(HDC hdc)
{
	HPEN hOldPen;
	list<Node*>::iterator iter;
	list<Node*>::iterator iterNext;
	if (bresenhamList.size() > 0)
	{
		//브래즌햄리스트에 있는 노드들을 순환하며
		//현재노드와 다음 노드를 이어준다
		hOldPen = (HPEN)SelectObject(hdc, g_hBresenhamPen);

		iter = bresenhamList.begin();
		iterNext = iter++;

		/*if (iter == nullptr)
			return;*/

		for (; iter != bresenhamList.end();)
		{
			int myX = GRID_SIZE / 2 + (*iter)->x * GRID_SIZE;
			int myY = GRID_SIZE / 2 + (*iter)->y * GRID_SIZE;
			int parentX = GRID_SIZE / 2 + (*iterNext)->x * GRID_SIZE;
			int parentY = GRID_SIZE / 2 + (*iterNext)->y * GRID_SIZE;

			MoveToEx(hdc, myX, myY, NULL);
			LineTo(hdc, parentX, parentY);

			iter++;
			iterNext++;
		}

		SelectObject(hdc, hOldPen);
	}
}

void DeleteAllOpenList()
{
	list<Node*>::iterator iter;
	for (iter = openList.begin(); iter != openList.end(); iter++)
	{
		delete (*iter);
	}

	openList.clear();
}

void DeleteAllCloseList()
{
	list<Node*>::iterator iter;
	for (iter = closeList.begin(); iter != closeList.end(); iter++)
	{
		delete (*iter);
	}

	closeList.clear();
}

void ClearMap()
{
	for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
	{
		for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
		{
			if(g_Tile[iCntH][iCntW] == START)
				continue;
			
			if (g_Tile[iCntH][iCntW] == END)
				continue;

			if (g_Tile[iCntH][iCntW] == OBSTACLE)
				continue;

			g_Tile[iCntH][iCntW] = NONE;
		}
	}
}

int GetDistance(int x1, int y1, int x2, int y2)
{
	return (int)sqrt(pow((x1 - x2) * COST, 2) + pow((y1 - y2) * COST, 2));
}

int GetManhattan(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) * COST + abs(y1 - y2) * COST;
}

bool CheckOpenList(const int x, const int y)
{
	list<Node*>::iterator iter;
	for (iter = openList.begin(); iter != openList.end(); iter++)
	{
		if ((*iter)->x == x && (*iter)->y == y)
		{
			return false;
		}
	}
	return true;
}

bool CheckCloseList(const int x, const int y)
{
	list<Node*>::iterator iter;
	for (iter = closeList.begin(); iter != closeList.end(); iter++)
	{
		if ((*iter)->x == x && (*iter)->y == y)
		{
			return false;
		}
	}
	return true;
}

Node* MakeNode(int x, int y, Node* parent/*Direction dir = OO*/)
{
	//오픈리스트 중복검사
	if (CheckOpenList(x, y) == false)
		return nullptr;
	//클로즈리스트 중복검사
	if (CheckCloseList(x, y) == false)
		return nullptr;

	Node* pNode = new Node;
	pNode->x = x;
	pNode->y = y;
	pNode->parent = parent;
	pNode->g = GetDistance(x, y, g_Start.iX, g_Start.iY);
	pNode->h = GetManhattan(x, y, g_end.iX, g_end.iY);
	pNode->f = pNode->g + pNode->h;

	//노드 방향 정하기
	if (parent == nullptr)
		pNode->dir = OO;
	else if (parent->x == x)
	{
		if (parent->y > y)
		{
			pNode->dir = UU;
		}
		else if (parent->y < y)
		{
			pNode->dir = DD;
		}
	}
	else if (parent->x > x)
	{
		if (parent->y > y)
		{
			pNode->dir = LU;
		}
		else if (parent->y < y)
		{
			pNode->dir = LD;
		}
		else if (parent->y == y)
		{
			pNode->dir = LL;
		}
	}
	else if (parent->x < x)
	{
		if (parent->y > y)
		{
			pNode->dir = RU;
		}
		else if (parent->y < y)
		{
			pNode->dir = RD;
		}
		else if (parent->y == y)
		{
			pNode->dir = RR;
		}
	}
	openList.push_front(pNode);

	return pNode;
}

void RenderTile(HDC hdc)
{
	int iX = 0;
	int iY = 0;

	HBRUSH hOldBrush = nullptr;
	//사각형의 테두리를 안보이도록 하기 위해 널펜을 지정한다
	//createPen으로  널펜을  생성 해도되지만 GetStockObject 를 사용하여 이미 시스템에 만들어져있는 고정 GDI오브젝트를 사용해본다
	//GetStockObject 는 시슽템의 고정적인 범용 GDI Object로서 삭제가 필요없다
	//시스템 전역적인 GDI Object 를 얻어서 사용한다는 개념.
	for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
	{
		for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
		{
			if (g_Tile[iCntH][iCntW] == NONE)
				continue;

			if (g_Tile[iCntH][iCntW] == OBSTACLE)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hTileBrush);
			}
			else if (g_Tile[iCntH][iCntW] == START)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hStartBrush);
			}
			else if (g_Tile[iCntH][iCntW] == END)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hendBrush);
			}
			else if (g_Tile[iCntH][iCntW] == CHECK)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hCheckBrush);
			}
			else if (g_Tile[iCntH][iCntW] == NODE)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hNodeBrush);
			}
			else if (g_Tile[iCntH][iCntW] == LINE)
			{
				hOldBrush = (HBRUSH)SelectObject(hdc, g_hLineBrush);
			}

			SelectObject(hdc, GetStockObject(NULL_PEN));
			iX = iCntW * GRID_SIZE;
			iY = iCntH * GRID_SIZE;
			//테두리 크기가 있으므로 +2 한다
			Rectangle(hdc, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);

		}
	}
	SelectObject(hdc, hOldBrush);
}

void CheckBresenham(Node* endNode)
{
	//끝노드 저장
	//bresenhamList.Push_Front(endNode);


	Node* sNode = endNode;
	Node* eNode = endNode->parent;
	Node* eNode_before = nullptr;
	int passCount = 0;

	while (sNode->x != g_Start.iX && sNode->y != g_Start.iY)
	{
		if (eNode == nullptr)
			return;

		//반복
		int oldX = endNode->x;
		int oldY = endNode->y;
		int curX = eNode->x;
		int curY = eNode->y;

		int h = curX - oldX;
		int v = curY - oldY;
		int big;
		int small;

		int xValue(0);
		int yValue(0);



		if (abs(h) > abs(v))
		{
			big = abs(h);
			small = abs(v);

			if (h > 0 && v < 0)
			{
				xValue = 1;
				yValue = -1;
			}
			else if (h > 0 && v > 0)
			{
				xValue = 1;
				yValue = 1;
			}
			else if (h < 0 && v < 0)
			{
				xValue = -1;
				yValue = -1;
			}
			else if (h < 0 && v > 0)
			{
				xValue = -1;
				yValue = 1;
			}
			else if (h > 0 && v == 0)
			{
				xValue = 1;
				//yValue = 1;
			}
			else if (h < 0 && v == 0)
			{
				xValue = -1;
				//yValue = 1;
			}
			int  i = 0;
			if (v < 0)
			{
				while (oldY > curY)
				{
					i += small;
					if (i > big)
					{
						i -= big;
						oldY += yValue;

						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}
							

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					else
					{
						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					oldX += xValue;
				}
			}
			else if (v == 0)
			{
				while (oldX != curX)
				{
					if (g_Tile[oldY][oldX] == OBSTACLE)
					{
						if (passCount > 0)
						{
							passCount = 0;
							bresenhamList.push_back(eNode_before);
							bresenhamList.push_back(sNode);
						}

						sNode = eNode_before;
						eNode = eNode->parent;
						return;
					}

					/*g_Tile[oldY][oldX] = LINE;
					InvalidateRect(hWnd, NULL, false);*/
					oldX += xValue;
				}
			}
			else
			{
				while (oldY < curY)
				{
					i += small;
					if (i > big)
					{
						i -= big;
						oldY += yValue;

						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					else
					{
						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					oldX += xValue;
				}
			}
		}
		else
		{
			big = abs(v);
			small = abs(h);

			if (h > 0 && v < 0)
			{
				xValue = 1;
				yValue = -1;
			}
			else if (h > 0 && v > 0)
			{
				xValue = 1;
				yValue = 1;
			}
			else if (h < 0 && v < 0)
			{
				xValue = -1;
				yValue = -1;
			}
			else if (h < 0 && v>0)
			{
				xValue = -1;
				yValue = 1;
			}
			else if (v > 0 && h == 0)
			{
				//xValue = 1;
				yValue = 1;
			}
			else if (v < 0 && h == 0)
			{
				//xValue = -1;
				yValue = -1;
			}

			int  i = 0;
			if (h < 0)
			{
				while (oldX > curX)
				{
					i += small;
					if (i > big)
					{
						i -= big;
						oldX += xValue;

						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					else
					{
						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					oldY += yValue;
				}
			}
			else if (h == 0)
			{
				while (oldY != curY)
				{
					if (g_Tile[oldY][oldX] == OBSTACLE)
					{
						if (passCount > 0)
						{
							passCount = 0;
							bresenhamList.push_back(eNode_before);
							bresenhamList.push_back(sNode);
						}

						sNode = eNode_before;
						eNode = eNode->parent;
						return;
					}

					/*g_Tile[oldY][oldX] = LINE;
					InvalidateRect(hWnd, NULL, false);*/
					oldY += yValue;
				}
			}
			else
			{
				while (oldX < curX)
				{
					i += small;
					if (i > big)
					{
						i -= big;
						oldX += xValue;

						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					else
					{
						if (g_Tile[oldY][oldX] == OBSTACLE)
						{
							if (passCount > 0)
							{
								passCount = 0;
								bresenhamList.push_back(eNode_before);
								bresenhamList.push_back(sNode);
							}

							sNode = eNode_before;
							eNode = eNode->parent;
							return;
						}

						/*g_Tile[oldY][oldX] = LINE;
						InvalidateRect(hWnd, NULL, false);*/
					}
					oldY += yValue;
				}
			}
		}

		//끝 타일 칠하기
		if (g_Tile[curY][curX] == OBSTACLE)
		{
			if (passCount > 0)
			{
				passCount = 0;
				bresenhamList.push_back(eNode_before);
				bresenhamList.push_back(sNode);
			}

			sNode = eNode_before;
			eNode = eNode->parent;
			return;
		}

		passCount++;
		eNode_before = eNode;
		eNode = eNode->parent;
		/*g_Tile[iTileY][iTileX] = LINE;*/
	}
}

void Findend(int x, int y, Node* node)
{
	isStarted = false;
	Node* tempNode = MakeNode(x, y, node);
	list<Node*>::iterator iter = openList.begin();
	openList.pop_front();
	closeList.pop_front();

	CheckBresenham(tempNode);

	while (tempNode != nullptr)
	{
		if (tempNode->parent != nullptr)
			wprintf_s(L"(***)(%d,%d) [p](%d,%d)\n", tempNode->x, tempNode->y, tempNode->parent->x, tempNode->parent->y);
		else
			wprintf_s(L"(***)(%d,%d)\n", tempNode->x, tempNode->y);

		tempNode = tempNode->parent;
	}
}

void CheckUU(int _x, int _y, Node* node = nullptr, bool isSubSearch = false)
{
	int x = _x;
	int y = _y - 1;
	for (; y >= 0; y--)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			if (isSubSearch == true)
			{
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
			}
			else
			{
				Findend(x, y, node);
			}

			return;
		}

		if (x == 0)
		{
			//오른쪽만 체크
			if (g_Tile[y][x + 1] == OBSTACLE && g_Tile[y - 1][x + 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}

		}
		else if(x==GRID_WIDTH)
		{
			//왼쪽만 체크
			if (g_Tile[y][x - 1] == OBSTACLE && g_Tile[y - 1][x - 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}
		//장애물 체크
		else if (g_Tile[y][x - 1] == OBSTACLE && g_Tile[y - 1][x - 1] != OBSTACLE || g_Tile[y][x + 1] == OBSTACLE && g_Tile[y - 1][x + 1] != OBSTACLE)
		{
			if (isSubSearch == true)
			{//보조 탐색일 경우 노드 생성 위치 다름
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
				g_Tile[y][x] = CHECK;
				return;
			}
			else
			{
				MakeNode(x, y, node);
				g_Tile[y][x] = NODE;
				return;
			}
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}
	}
}

void CheckRR(int _x, int _y, Node* node = nullptr, bool isSubSearch = false)
{
	int x = _x + 1;
	int y = _y;
	for (; x < GRID_WIDTH; x++)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			if (isSubSearch == true)
			{
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
			}
			else
			{
				Findend(x, y, node);
			}
			return;
		}

		if (y == 0)
		{
			//아래만 체크
			if (g_Tile[y + 1][x] == OBSTACLE && g_Tile[y + 1][x + 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}
		else if (y == GRID_HEIGHT - 1)
		{
			//위만 체크
			if (g_Tile[y - 1][x] == OBSTACLE && g_Tile[y - 1][x + 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}
		//장애물 체크
		else if (g_Tile[y - 1][x] == OBSTACLE && g_Tile[y - 1][x + 1] != OBSTACLE || g_Tile[y + 1][x] == OBSTACLE && g_Tile[y + 1][x + 1] != OBSTACLE)
		{
			if (isSubSearch == true)
			{//보조 탐색일 경우 노드 생성 위치 다름
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
				g_Tile[y][x] = CHECK;
				return;
			}
			else
			{
				MakeNode(x, y, node);
				g_Tile[y][x] = NODE;
				return;
			}
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}
	}
}

void CheckDD(int _x, int _y, Node* node = nullptr, bool isSubSearch = false)
{
	int x = _x;
	int y = _y + 1;
	for (; y < GRID_HEIGHT; y++)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y )
		{
			if (isSubSearch == true)
			{
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
			}
			else
			{
				Findend(x, y, node);
			}
			return;
		}

		if (x == 0)
		{
			//오른쪽만 체크
			if (g_Tile[y][x + 1] == OBSTACLE && g_Tile[y + 1][x + 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}
		else if (x == GRID_WIDTH)
		{
			//왼쪽만 체크
			if (g_Tile[y][x - 1] == OBSTACLE && g_Tile[y + 1][x - 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}
		//장애물 체크
		else if (g_Tile[y][x - 1] == OBSTACLE && g_Tile[y + 1][x - 1] != OBSTACLE || g_Tile[y][x + 1] == OBSTACLE && g_Tile[y + 1][x + 1] != OBSTACLE)
		{
			if (isSubSearch == true)
			{//보조 탐색일 경우 노드 생성 위치 다름
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
				g_Tile[y][x] = CHECK;
				return;
			}
			else
			{
				MakeNode(x, y, node);
				g_Tile[y][x] = NODE;
				return;
			}
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}
	}
}

void CheckLL(int _x, int _y, Node* node = nullptr, bool isSubSearch = false)
{
	int x = _x - 1;
	int y = _y;
	for (; x >= 0; x--)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			if (isSubSearch == true)
			{
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
			}
			else
			{
				Findend(x, y, node);
			}
			return;
		}

		if (y == 0)
		{
			//아래만 체크
			if (g_Tile[y + 1][x] == OBSTACLE && g_Tile[y + 1][x - 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}
		else if (y == GRID_HEIGHT - 1)
		{
			//위만 체크
			if (g_Tile[y - 1][x] == OBSTACLE && g_Tile[y - 1][x - 1] != OBSTACLE)
			{
				if (isSubSearch == true)
				{//보조 탐색일 경우 노드 생성 위치 다름
					MakeNode(_x, _y, node);
					g_Tile[_y][_x] = NODE;
					g_Tile[y][x] = CHECK;
					return;
				}
				else
				{
					MakeNode(x, y, node);
					g_Tile[y][x] = NODE;
					return;
				}
			}
			else
			{
				g_Tile[y][x] = CHECK;
			}
		}

		//장애물 체크
		else if (g_Tile[y - 1][x] == OBSTACLE && g_Tile[y - 1][x - 1] != OBSTACLE || g_Tile[y + 1][x] == OBSTACLE && g_Tile[y + 1][x - 1] != OBSTACLE)
		{
			if (isSubSearch == true)
			{//보조 탐색일 경우 노드 생성 위치 다름
				MakeNode(_x, _y, node);
				g_Tile[_y][_x] = NODE;
				g_Tile[y][x] = CHECK;
				return;
			}
			else
			{
				MakeNode(x, y, node);
				g_Tile[y][x] = NODE;
				return;
			}
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}
	}
}

void CheckRU(Node* node)
{
	int x = node->x + 1;
	int y = node->y - 1;
	for (; x < GRID_WIDTH && y >= 0;)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			Findend(x, y, node);
			return;
		}

		//장애물 체크
		if (g_Tile[y][x - 1] == OBSTACLE && g_Tile[y - 1][x - 1] != OBSTACLE ||
			g_Tile[y + 1][x] == OBSTACLE && g_Tile[y + 1][x + 1] != OBSTACLE)
		{
			MakeNode(x, y, node);
			g_Tile[y][x] = NODE;
			x++;
			y--;
			return;
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}

		//수직 체크
		CheckUU(x, y, node, true);
		CheckRR(x, y, node, true);

		x++;
		y--;
	}
}

void CheckRD(Node* node)
{
	int x = node->x + 1;
	int y = node->y + 1;
	for (; x < GRID_WIDTH && y < GRID_HEIGHT;)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			Findend(x, y, node);
			return;
		}

		//장애물 체크
		if (g_Tile[y][x - 1] == OBSTACLE && g_Tile[y + 1][x - 1] != OBSTACLE ||
			g_Tile[y - 1][x] == OBSTACLE && g_Tile[y - 1][x + 1] != OBSTACLE)
		{
			MakeNode(x, y, node);
			g_Tile[y][x] = NODE;
			x++;
			y++;
			return;
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}

		//수직 체크
		CheckDD(x, y, node, true);
		CheckRR(x, y, node, true);

		x++;
		y++;
	}
}

void CheckLD(Node* node)
{
	int x = node->x - 1;
	int y = node->y + 1;
	for (; x >= 0 && y < GRID_HEIGHT;)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			Findend(x, y, node);
			return;
		}

		//장애물 체크
		if (g_Tile[y-1][x] == OBSTACLE && g_Tile[y - 1][x - 1] != OBSTACLE ||
			g_Tile[y][x+1] == OBSTACLE && g_Tile[y + 1][x + 1] != OBSTACLE)
		{
			MakeNode(x, y, node);
			g_Tile[y][x] = NODE;
			x--;
			y++;
			return;
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}

		//수직 체크
		CheckDD(x, y, node, true);
		CheckLL(x, y, node, true);

		x--;
		y++;
	}
}

void CheckLU(Node* node)
{
	int x = node->x - 1;
	int y = node->y - 1;
	for (; x >=0 && y >= 0;)
	{
		//시작지점인지 확인
		if (g_Start.iX == x && g_Start.iY == y)
		{
			return;
		}

		//장애물인지 확인
		if (g_Tile[y][x] == OBSTACLE)
			return;

		//도착지인지 확인
		if (g_end.iX == x && g_end.iY == y)
		{
			Findend(x, y, node);
			return;
		}

		//장애물 체크
		if (g_Tile[y + 1][x] == OBSTACLE && g_Tile[y + 1][x - 1] != OBSTACLE ||
			g_Tile[y][x + 1] == OBSTACLE && g_Tile[y - 1][x + 1] != OBSTACLE)
		{
			MakeNode(x, y, node);
			g_Tile[y][x] = NODE;
			x--;
			y--;
			return;
		}
		else
		{
			g_Tile[y][x] = CHECK;
		}

		//수직 체크
		CheckUU(x, y, node, true);
		CheckLL(x, y, node, true);

		x--;
		y--;
	}
}

void SortList()
{
	list<Node*>::iterator iter;

	//버블정렬
	for (size_t i = openList.size(); i > 0; i--)
	{
		iter = openList.begin();

		for (int j = 0; j < i; j++)
		{
			list<Node*>::iterator iterNext = iter;
			iterNext++;
			if (iterNext == openList.end())
				break;

			if ((*iter)->f > (*iterNext)->f)
			{
				Node* temp = *iter;
				*iter = *iterNext;
				*iterNext = temp;
			}

			iter++;
		}
	}

	for (iter = openList.begin(); iter != openList.end(); iter++)
	{
		wprintf_s(L"f:%d (%d,%d) %p\n", (*iter)->f, (*iter)->x, (*iter)->y, *iter);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//wprintf_s(L"openListsize: %d, closeListsize: %d\n", openList.size(), closeList.size());

	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_CONTROL)
		{
			InvalidateRect(hWnd, NULL, false);

			if (isLine == true)
				break;

			g_OldPos.iX = g_CurrentPos.iX;
			g_OldPos.iY = g_CurrentPos.iY;

			g_OldTile.iX = g_OldPos.iX / GRID_SIZE;
			g_OldTile.iY = g_OldPos.iY / GRID_SIZE;

			g_Tile[g_OldTile.iY][g_OldTile.iX] = LINE;

			isLine = true;
		}
	}
	break;
	case WM_KEYUP:
	{
		if (wParam == VK_CONTROL)
		{
			if (isLine == false)
				break;

			isLine = false;

			//모든 LINE 타일들 NONE으로 바꾸기 
			for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
			{
				for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
				{
					if (g_Tile[iCntH][iCntW] == LINE)
						g_Tile[iCntH][iCntW] = NONE;
				}
			}

			InvalidateRect(hWnd, NULL, false);
		}
	}
	break;

	//case WM_MBUTTONDOWN:
	//{
	//	//휠클릭

	//	//시작이 안됬으면 시작 및 초기화
	//	if (isStarted == false)
	//	{
	//		DeleteAllOpenList();
	//		DeleteAllCloseList();
	//		bresenhamList.Clear();
	//		ClearMap();
	//		isStarted = true;
	//		//시작노드를 만들고 오픈리스트 추가
	//		MakeNode(g_Start.iX, g_Start.iY, nullptr);

	//	}
	//}
	//break;
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
		if (g_Start.iX == -1)
		{
			g_Tile[ClickPos.iY][ClickPos.iX] = START;
			g_Start(ClickPos.iX, ClickPos.iY);
		}
		//시작타일 있는데 끝타일이 없으면 -> 끝타일을 만든다.
		else if (g_Start.iX != -1 && g_end.iX == -1)
		{
			g_Tile[ClickPos.iY][ClickPos.iX] = END;
			g_end(ClickPos.iX, ClickPos.iY);
		}
		//시작타일, 끝타일 모두 있으면 -> 시작타일의 위치를 바꾸고 끝타일을 제거
		else if(g_Start.iX != -1 && g_end.iX != -1)
		{
			g_Tile[g_end.iY][g_end.iX] = NONE;
			g_end(-1, -1);

			g_Tile[g_Start.iY][g_Start.iX] = NONE;
			g_Start(ClickPos.iX, ClickPos.iY);

			g_Tile[ClickPos.iY][ClickPos.iX] = START;

			
		}
		InvalidateRect(hWnd, NULL, false);
	}
	break;
	case WM_LBUTTONDOWN:
		g_bDrag = true;
		{
			POS ClickPos(LOWORD(lParam) / GRID_SIZE, HIWORD(lParam) / GRID_SIZE);
			/*int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			int iTileX = xPos / GRID_SIZE;
			int iTileY = yPos / GRID_SIZE;*/
			//첫 선택 타일이 장애물이면 지우기모드 아니면 장애물 넣기 모드
			if (g_Tile[ClickPos.iY][ClickPos.iX] == OBSTACLE)
			{
				g_bErase = true;
			}
			else
			{
				g_bErase = false;
			}
		}
		break;

	case WM_LBUTTONUP:
		g_bDrag = false;
		break;

	case WM_MOUSEMOVE:
	{
		//마우스 현재위치 갱신
		g_CurrentPos.iX = LOWORD(lParam);
		g_CurrentPos.iY = HIWORD(lParam);

		if (isLine == true)
		{
			//모든 LINE 타일들 NONE으로 바꾸기 (최초 시작점 빼고)
			for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
			{
				for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
				{
					if (iCntW == g_OldTile.iX && iCntH == g_OldTile.iY)
						continue;

					if (g_Tile[iCntH][iCntW] == LINE)
						g_Tile[iCntH][iCntW] = NONE;
				}
			}

			int iTileX = g_CurrentPos.iX / GRID_SIZE;
			int iTileY = g_CurrentPos.iY / GRID_SIZE;

			InvalidateRect(hWnd, NULL, false);
			
		}
		//중간 타일 그리기///////////////////////////////
		if (g_bDrag)
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);

			int iTileX = xPos / GRID_SIZE;
			int iTileY = yPos / GRID_SIZE;

			if (iTileX<0 || iTileX>GRID_WIDTH || iTileY<0 || iTileY>GRID_HEIGHT)//다른 전역변수 건들이는 것 방지
				return 0;

			g_Tile[iTileY][iTileX] = !g_bErase;
			InvalidateRect(hWnd, NULL, false);
		}
		//...기존 코드들

		//InvalidateRect(hWnd, NULL, false);
		//마우스 드래그로 데이터가 변경되어 갱신을 요청 할 시 마지막 Erase플래그를 false로 하여 화면
		//깜빡임을 없앤다. WM_PAINT에서는 윈도우 전체를 덮어 쓰기 때문에 지우지 않아도 된다.

	}
	break;

	case WM_DESTROY:
		SelectObject(g_hMemDC, g_hMemDCBitmap_old);
		DeleteObject(g_hMemDC);
		DeleteObject(g_hMemDCBitmap);

		PostQuitMessage(0);
		break;

	case WM_CREATE:
		//윈도우 생성 시 현 윈도우 크기와 동일한 메모리DC생성
	{
		HDC hdc = GetDC(hWnd);
		GetClientRect(hWnd, &g_MemDCRect);
		g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
		g_hMemDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);
		g_hMemDCBitmap_old = (HBITMAP)SelectObject(g_hMemDC, g_hMemDCBitmap);
	}
	//
	g_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	g_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));
	g_hStartBrush = CreateSolidBrush(RGB(0, 200, 0));
	g_hendBrush = CreateSolidBrush(RGB(200, 0, 0));
	g_hCheckBrush = CreateSolidBrush(RGB(100, 100, 200));
	g_hNodeBrush = CreateSolidBrush(RGB(200, 200, 0));
	g_hParentPen_Open = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
	g_hParentPen_Close = CreatePen(PS_SOLID, 1, RGB(200, 0, 0));
	g_hLineBrush = CreateSolidBrush(RGB(200, 200, 100));
	g_hBresenhamPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 200));

	///
	/*srand(time(NULL));*/
	///
	break;

	case WM_PAINT:
	{
		//메모리 DC를 클리어하고
		PatBlt(g_hMemDC, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, WHITENESS);

		//RenderObstacle, RenderGrid를 메모리 DC에 출력
		RenderTile(g_hMemDC);
		RenderGrid(g_hMemDC);
		RenderParent(g_hMemDC);
		RenderBresenhamLine(g_hMemDC);

		if (isLine == true)
		{
			RenderLine(g_hMemDC);
		}

		//메모리 DC에 랜더링이 끝나면, 메모리 DC -> 윈도우 DC로의 출력
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_SIZE:
	{
		SelectObject(g_hMemDC, g_hMemDCBitmap_old);
		DeleteObject(g_hMemDC);
		DeleteObject(g_hMemDCBitmap);

		HDC hdc = GetDC(hWnd);

		GetClientRect(hWnd, &g_MemDCRect);
		g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
		g_hMemDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);

		g_hMemDCBitmap_old = (HBITMAP)SelectObject(g_hMemDC, g_hMemDCBitmap);
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
