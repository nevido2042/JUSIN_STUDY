#pragma once

#include "MyTerrain.h"

class CAstarMgr
{
public:
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE*>& Get_BestList() { return m_BestList; }

public:
	void		Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);
	void		Render();
	void		Release();

private:
	bool		Make_Route(int iStartIdx, int iGoalIdx);
	void		Make_BestList(int iStartIdx, int iGoalIdx);
	//베스트 리스트에서 최적 경로로 가공하는 함수
	void		Find_ShortestPath_Bresenham();
	bool		Check_Obstacle_Bresenham(int _iStartIndex, int _iEndIndex);
	bool		IsObstacle(int _iX, int _iY);
	
	int			Get_TileIdx(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	bool		Check_Close(int iIndex);
	bool		Check_Open(int iIndex);
	void		Render_BestList();
	void		Render_BestestList();
private:
	list<int>		m_OpenList;
	list<int>		m_CloseList;

	list<TILE*>		m_BestList;
	list<TILE*>		m_BestestList;

	int				m_iStartIdx;
};

