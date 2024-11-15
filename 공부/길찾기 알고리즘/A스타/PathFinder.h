#pragma once
#include "pch.h"
#include "Define.h"
#include "Map.h"
#include <list>
#include "Node.h"
//#include <queue>

class CPathFinder
{
	CMap* m_pMap;
	POS m_StartPos;
	POS m_EndPos;

	list<CNode*> m_OpenList;
	list<CNode*> m_CloseList;

public:
	CPathFinder();
	~CPathFinder();
public:
	void Initialize(CMap& _pMap);
	void Release();
private:
	bool Is_Position_InMap(POS _Pos);
	static bool LessF(const CNode* _First, const CNode* _Second);
	void Search_Corner(CNode& _Node);//코너를 찾는 함수
	void Search_Linear(CNode& _Node, DIRECTION _Dir);//노드의 방향에 따른 직선 탐색 함수
	void Search_Direction(const CNode& _Node, DIRECTION _Dir);//한 방향 탐색 함수
	void Search_Direction(const POS _Pos, const CNode& _Parent, DIRECTION _Dir);//한 방향 탐색 함수
	void Search_Diagonal(CNode& _Node, DIRECTION _Dir);//노드의 방향에 따른 대각선 탐색 함수
public:
	void Start_A_Star();
	void Start_JPS();
};

