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

