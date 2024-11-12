#include "TileManager.h"

void CTileManager::Initialize()
{
	m_StartPos();
	m_EndPos();

	m_hGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	m_hTileBrush = CreateSolidBrush(RGB(100, 100, 100));
	m_hStartBrush = CreateSolidBrush(RGB(0, 200, 0));
	m_hEndBrush = CreateSolidBrush(RGB(200, 0, 0));
}

void CTileManager::RenderTile()
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
			if (m_Tile[iCntH][iCntW] == NONE)
				continue;

			if (m_Tile[iCntH][iCntW] == OBSTACLE)
			{
				hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hTileBrush);
			}
			else if (m_Tile[iCntH][iCntW] == START)
			{
				hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hStartBrush);
			}
			else if (m_Tile[iCntH][iCntW] == END)
			{
				hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hEndBrush);
			}

			SelectObject(m_hDC, GetStockObject(NULL_PEN));
			iX = iCntW * GRID_SIZE;
			iY = iCntH * GRID_SIZE;
			//테두리 크기가 있으므로 +2 한다
			Rectangle(m_hDC, iX, iY, iX + GRID_SIZE + 2, iY + GRID_SIZE + 2);

		}
	}
	SelectObject(m_hDC, hOldBrush);
}

void CTileManager::RenderGrid()
{
	int iX = 0;
	int iY = 0;
	HPEN hOldPen = (HPEN)SelectObject(m_hDC, m_hGridPen);
	//그리드의 마지막 선을 추가로 그리기 위해 <=의 반복 조건
	for (int iCntW = 0; iCntW <= GRID_WIDTH; iCntW++)
	{
		MoveToEx(m_hDC, iX, 0, NULL);
		LineTo(m_hDC, iX, GRID_HEIGHT * GRID_SIZE);
		iX += GRID_SIZE;
	}
	for (int iCntH = 0; iCntH <= GRID_HEIGHT; iCntH++)
	{
		MoveToEx(m_hDC, 0, iY, NULL);
		LineTo(m_hDC, GRID_WIDTH * GRID_SIZE, iY);
		iY += GRID_SIZE;
	}
	SelectObject(m_hDC, hOldPen);
}
