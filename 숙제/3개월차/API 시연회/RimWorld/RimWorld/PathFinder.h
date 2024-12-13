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

	void Search_Corner(const CNode& _Node, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEndPos);

	void Search_Linear(const CNode& _Node, const DIRECTION _Dir, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEndPos);

	void Search_Direction(const CNode& _Node, const DIRECTION _Dir, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEnd, const POS _Pos = POS(-999, -999));

	void Search_Diagonal(const CNode& _Node, const DIRECTION _Dir, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEnd);

	bool Check_Visit(const POS& _Pos, const list<CNode*>* pOpenList, const list<CNode*>* pCloseList);

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

