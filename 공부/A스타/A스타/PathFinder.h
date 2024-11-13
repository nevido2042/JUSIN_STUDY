#pragma once
#include "pch.h"
#include "Define.h"
#include "Map.h"

class CPathFinder
{
	CMap* m_pMap;
	POS m_StartPos;
	POS m_EndPos;
public:
	CPathFinder();
public:
	void Initialize(CMap& _pMap);

};

