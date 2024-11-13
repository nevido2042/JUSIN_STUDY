#include "pch.h"
#include "PathFinder.h"

CPathFinder::CPathFinder():m_pMap(nullptr)
{
}

void CPathFinder::Initialize(CMap& _pMap)
{
	m_pMap = &_pMap;
	//m_StartPos = _pMap.m_StartPos; 이런느낌으로
}
