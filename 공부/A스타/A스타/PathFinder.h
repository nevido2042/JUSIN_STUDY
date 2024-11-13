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
	static bool LessF(const CNode* _First, const CNode* _Second);
public:
	void Start();
};

