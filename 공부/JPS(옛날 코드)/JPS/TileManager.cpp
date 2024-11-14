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
	//�簢���� �׵θ��� �Ⱥ��̵��� �ϱ� ���� ������ �����Ѵ�
	//createPen����  ������  ���� �ص������� GetStockObject �� ����Ͽ� �̹� �ý��ۿ� ��������ִ� ���� GDI������Ʈ�� ����غ���
	//GetStockObject �� �Ú����� �������� ���� GDI Object�μ� ������ �ʿ����
	//�ý��� �������� GDI Object �� �� ����Ѵٴ� ����.
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
			//�׵θ� ũ�Ⱑ �����Ƿ� +2 �Ѵ�
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
	//�׸����� ������ ���� �߰��� �׸��� ���� <=�� �ݺ� ����
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
