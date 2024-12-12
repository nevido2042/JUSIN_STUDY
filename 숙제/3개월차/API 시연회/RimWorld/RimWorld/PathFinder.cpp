#include "pch.h"
#include "PathFinder.h"
#include "Node.h"
#include "TileMgr.h"

CPathFinder* CPathFinder::m_pInstance = nullptr;

CPathFinder::CPathFinder()
{
}

CPathFinder::~CPathFinder()
{
}

list<CNode*> CPathFinder::Find_Path(POS _tStart, POS _tEnd)
{
	list<CNode*> OpenList;
	list<CNode*> CloseList;

	CNode* StartNode = new CNode(_tStart);
	OpenList.push_back(StartNode);

	while (!OpenList.empty())
	{
		//�ϳ� �̴´�.
		CNode* pPopNode = OpenList.front();
		OpenList.pop_front();

		//Ŭ���� ����Ʈ�� �ִ´�.
		CloseList.push_back(pPopNode);

		//���� �������� Ȯ���Ѵ�.
		if (CNode::Distance(_tEnd, pPopNode->Get_Pos()) < TILECX)//TILCX ���� ������ �����Ѱɷ� ģ��...�³�?
		{

			for_each(OpenList.begin(), OpenList.end(), Safe_Delete<CNode*>);
			OpenList.clear();

			//Ŭ�����Ʈ�� ��ȯ �ؾ��ұ�? ����Ʈ�� �Ѱ�����ұ�?

			return CloseList;
		}

		//8���� Ž���Ѵ�.
		int iDX[8]{ 0, 1, 1, 1,	0, -1, -1, -1 };
		int iDY[8]{ -1,	-1, 0, 1, 1, 1,	0, -1 };
		//  12   1  3  4  6   7   9  11
		
		for (int i = 0; i < 8; ++i)
		{
			//CTileMgr::Get_Instance()->Get_TileArray().at(1);
			float fX = float(pPopNode->Get_Pos().fX + iDX[i] * TILECX);
			float fY = float(pPopNode->Get_Pos().fY + iDY[i] * TILECY);

			//�� ���ΰ�?
			if (fX < 0 || fY < 0 || TILECX * TILEX < fX || TILECY * TILEY < fY)
			{
				continue;
			}

			POS NewPos(fX, fY);

			//fx fy�� �ε��� ���
			int iIdx = int(fX / TILECX) + int((fY / TILECY) * TILEX);

			//���ΰ�?
			CTile* pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->
				Get_TileArray().at(iIdx));

			if (!pTile || pTile->Get_Option() == OPT_BLOCKED)
			{
				continue;
			}

			//Ŭ���� ����Ʈ �Ǵ� ���¸���Ʈ�� ���� �ϴ°�?
			bool bIsNewPos = true;
			for (list<CNode*>::iterator iter = OpenList.begin(); iter != OpenList.end(); ++iter)
			{
				if (NewPos == (*iter)->Get_Pos())
				{
					bIsNewPos = false;
					break;
				}
			}
			for (list<CNode*>::iterator iter = CloseList.begin(); iter != CloseList.end(); ++iter)
			{
				if (NewPos == (*iter)->Get_Pos())
				{
					bIsNewPos = false;
					break;
				}
			}
			if (bIsNewPos)
			{
				CNode* pNewNode = new CNode(NewPos, pPopNode, _tEnd);
				OpenList.push_back(pNewNode);
			}
		}

		//���� ����Ʈ ����
		OpenList.sort(LessF);

		//m_pMap->Update();
	}

	for_each(OpenList.begin(), OpenList.end(), Safe_Delete<CNode*>);
	OpenList.clear();

	for_each(CloseList.begin(), CloseList.end(), Safe_Delete<CNode*>);
	CloseList.clear();

	return CloseList;//����
}

bool CPathFinder::LessF(const CNode* _First, const CNode* _Second)
{
	return _First->Get_F() < _Second->Get_F();
}
