#pragma once
#include "Obj.h"
#include "Node.h"

class CPathFinder
{
private:
	CPathFinder();
	~CPathFinder();

public:
	list<CNode*> Find_Path(POS _tStart, POS _tEnd);

private:
	static bool LessF(const CNode* _First, const CNode* _Second);

	void Search_Corner(CNode& _Node, list<CNode*>* pOpenList);

	void Search_Linear(CNode& _Node, DIRECTION _Dir, list<CNode*>* pOpenList);

	void Search_Direction(const CNode& _Node, DIRECTION _Dir, list<CNode*>* pOpenList, const POS _Pos = POS(-999, -999));

	void Search_Diagonal(CNode& _Node, DIRECTION _Dir, list<CNode*>* pOpenList);

	bool Check_Visit(POS& _Pos, list<CNode*>* pOpenList, list<CNode*>* pCloseList);

public:
	static CPathFinder* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CPathFinder;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CPathFinder* m_pInstance;
};

