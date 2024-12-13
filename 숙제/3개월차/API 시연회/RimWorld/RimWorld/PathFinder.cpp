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
		//��带 �ϳ� �̴´�.
		CNode* PopNode = OpenList.front();
		OpenList.pop_front();

		//Ŭ���� ����Ʈ�� �ִ´�.
		CloseList.push_back(PopNode);

		//���� �����ΰ�?
		if (_tEnd == PopNode->Get_Pos())
		{
			// �θ� �̾ ���� �����.
			const CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//���������� ��ĥ���� �ʵ���
				if (pParent->Get_Pos() == _tStart)
					break;
			}

			for_each(OpenList.begin(), OpenList.end(), Safe_Delete<CNode*>);
			OpenList.clear();

			return CloseList;
		}

		//�θ��� �������� Ž��(ù ���� 8��Ž��)
		// - ����Ž��, �밢�� Ž��
		//�ڳʸ� ã�� ���� ����Ʈ�� �ִ´�.
		Search_Corner(*PopNode, &OpenList, &CloseList, _tEnd);

		//�����Ѵ�.
		OpenList.sort(LessF);
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

void CPathFinder::Search_Corner(const CNode& _Node, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEndPos)
{
	switch (_Node.Get_Direction())
	{
	case OO:
		Search_Linear(_Node, UU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, RU, pOpenList, pCloseList, _tEndPos);
		Search_Linear(_Node, RR, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, RD, pOpenList, pCloseList, _tEndPos);
		Search_Linear(_Node, DD, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LD, pOpenList, pCloseList, _tEndPos);
		Search_Linear(_Node, LL, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LU, pOpenList, pCloseList, _tEndPos);
		break;
	case UU:
		Search_Linear(_Node, UU, pOpenList, pCloseList, _tEndPos);

		break;
	case RU:
		Search_Diagonal(_Node, RU, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RD, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LU, pOpenList, pCloseList, _tEndPos);

		break;
	case RR:
		Search_Linear(_Node, RR, pOpenList, pCloseList, _tEndPos);

		break;
	case RD:
		Search_Diagonal(_Node, RD, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LD, pOpenList, pCloseList, _tEndPos);

		break;
	case DD:
		Search_Linear(_Node, DD, pOpenList, pCloseList, _tEndPos);

		break;
	case LD:
		Search_Diagonal(_Node, LD, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, LU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, RD, pOpenList, pCloseList, _tEndPos);
		break;
	case LL:
		Search_Linear(_Node, LL, pOpenList, pCloseList, _tEndPos);

		break;
	case LU:
		Search_Diagonal(_Node, LU, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LD, pOpenList, pCloseList, _tEndPos);
		break;
	default:
		break;
	}
}

void CPathFinder::Search_Linear(const CNode& _Node, const DIRECTION _Dir, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEndPos)
{
	switch (_Dir)
	{
	case UU:
		Search_Direction(_Node, UU, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LU, pOpenList, pCloseList, _tEndPos);
		break;

	case RR:
		Search_Direction(_Node, RR, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, RD, pOpenList, pCloseList, _tEndPos);
		break;

	case DD:
		Search_Direction(_Node, DD, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RD, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LD, pOpenList, pCloseList, _tEndPos);
		break;

	case LL:
		Search_Direction(_Node, LL, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, LD, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LU, pOpenList, pCloseList, _tEndPos);
		break;

	default:
		break;
	}
}

void CPathFinder::Search_Direction(const CNode& _Node, const DIRECTION _Dir, list<CNode*>* pOpenList,  const list<CNode*>* pCloseList,  const POS _tEnd, const POS _Pos)
{
	POS Dir;
	POS Wall1_Dir;
	POS Road1_Dir;
	POS Wall2_Dir;
	POS Road2_Dir;

	switch (_Dir)
	{
	case UU:
		Dir = POS(0, -1) * TILECX;

		Wall1_Dir = POS(-1, 0) * TILECX;
		Road1_Dir = POS(-1, -1) * TILECX;
		Wall2_Dir = POS(1, 0) * TILECX;
		Road2_Dir = POS(1, -1) * TILECX;
		break;

	case RR:
		Dir = POS(1, 0) * TILECX;

		Wall1_Dir = POS(0, -1) * TILECX;
		Road1_Dir = POS(1, -1) * TILECX;
		Wall2_Dir = POS(1, 0) * TILECX;
		Road2_Dir = POS(1, -1) * TILECX;
		break;

	case DD:
		Dir = POS(0, 1) * TILECX;

		Wall1_Dir = POS(1, 0) * TILECX;
		Road1_Dir = POS(1, 1) * TILECX;
		Wall2_Dir = POS(-1, 0) * TILECX;
		Road2_Dir = POS(-1, 1) * TILECX;
		break;

	case LL:
		Dir = POS(-1, 0) * TILECX;

		Wall1_Dir = POS(0, 1) * TILECX;
		Road1_Dir = POS(-1, 1) * TILECX;
		Wall2_Dir = POS(0, -1) * TILECX;
		Road2_Dir = POS(-1, -1) * TILECX;

		break;

	default:
		break;
	}

	bool bIsSub_Search(false);
	POS Sub_Search_Pos;
	POS CheckPos;
	if (_Pos == POS(-999, -999))
	{
		CheckPos = _Node.Get_Pos();
		bIsSub_Search = false;
	}
	else
	{
		CheckPos = _Pos;
		bIsSub_Search = true;
		Sub_Search_Pos = CheckPos;
	}

	//����
	CheckPos.fX += Dir.fX;
	CheckPos.fY += Dir.fY;

	while (true)
	{
		POS Wall1(CheckPos.fX + Wall1_Dir.fX, CheckPos.fY + Wall1_Dir.fY);
		POS Road1(CheckPos.fX + Road1_Dir.fX, CheckPos.fY + Road1_Dir.fY);
		POS Wall2(CheckPos.fX + Wall2_Dir.fX, CheckPos.fY + Wall2_Dir.fY);
		POS Road2(CheckPos.fX + Road2_Dir.fX, CheckPos.fY + Road2_Dir.fY);

		//�� ���ΰ�?
		if (!CTileMgr::Get_Instance()->IsValidTile(CheckPos))
		{
			break;
		}

		//üũ�ϴ� ��ġ�� ������ ���� Ȯ��
		if (CNode::Distance(_tEnd, CheckPos) < TILECX * 0.5f)//TILCX ���� ������ �����Ѱɷ� ģ��...�³�?
		{
			if (Check_Visit(CheckPos, pOpenList, pCloseList))
			{
				break;
			}

			POS New_Node_Pos;
			if (bIsSub_Search)
			{
				New_Node_Pos = Sub_Search_Pos;
			}
			else
			{
				New_Node_Pos = CheckPos;
			}

			pOpenList->push_back(new CNode(New_Node_Pos, &_Node, _tEnd, _Dir));

			break;
		}

		//fx fy�� �ε��� ���
		int iIdx = int(CheckPos.fX / TILECX) + int(CheckPos.fY / TILECY) * TILEX;

		//if (TILEX * TILEY <= iIdx)
		//{
		//	continue;
		//	//���� Ÿ�� ��� ���̵��� ����? ex 600 611
		//}

		//���ΰ�?
		CTile* pTile = dynamic_cast<CTile*>(CTileMgr::Get_Instance()->
			Get_TileArray().at(iIdx));

		if (pTile->Get_Option() == OPT_BLOCKED)
		{
			break; //continue
		}


		POS New_Node_Pos;
		if (bIsSub_Search)
		{
			New_Node_Pos = Sub_Search_Pos;
		}
		else
		{
			New_Node_Pos = CheckPos;
		}

		//���¸���Ʈ �Ǵ� Ŭ���� ����Ʈ�� �̹� �ִ� �������� Ȯ��
		if (Check_Visit(New_Node_Pos, pOpenList, pCloseList))
		{
			break;
		}

		//�ֺ��� �ڳʰ� �ִ��� üũ
		bool bIsNewNode(false);
		if (CTileMgr::Get_Instance()->IsValidTile(Wall1) && CTileMgr::Get_Instance()->IsValidTile(Road1))
		{
			if (CTileMgr::Get_Instance()->Get_TileOption(Wall1) == OPT_BLOCKED && CTileMgr::Get_Instance()->Get_TileOption(Road1) == OPT_REACHABLE)
			{
				pOpenList->push_back(new CNode(New_Node_Pos, &_Node, _tEnd, _Dir));

				bIsNewNode = true;
			}
		}

		if (CTileMgr::Get_Instance()->IsValidTile(Wall2) && CTileMgr::Get_Instance()->IsValidTile(Road2))
		{
			if (CTileMgr::Get_Instance()->Get_TileOption(Wall2) == OPT_BLOCKED && CTileMgr::Get_Instance()->Get_TileOption(Road2) == OPT_REACHABLE)
			{
				POS New_Node_Pos;
				if (bIsSub_Search)
				{
					New_Node_Pos = Sub_Search_Pos;
				}
				else
				{
					New_Node_Pos = CheckPos;
				}

				pOpenList->push_back(new CNode(New_Node_Pos, &_Node, _tEnd, _Dir));

				bIsNewNode = true;
			}
		}

		//�ڳʰ� ������ ��� ���� �� break;
		if (bIsNewNode)
		{
			break;
		}

		//����
		CheckPos.fX += Dir.fX;
		CheckPos.fY += Dir.fY;
	}
}

void CPathFinder::Search_Diagonal(const CNode& _Node, const DIRECTION _Dir, list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEnd)
{
	POS Dir;
	POS Wall1_Dir;
	POS Road1_Dir;
	POS Wall2_Dir;
	POS Road2_Dir;

	DIRECTION Sub_Search1 = OO;
	DIRECTION Sub_Search2 = OO;

	switch (_Dir)
	{

	case RU:
		Dir = POS(1, -1) * TILECX;

		Wall1_Dir = POS(-1, 0) * TILECX;
		Road1_Dir = POS(-1, -1) * TILECX;
		Wall2_Dir = POS(0, 1) * TILECX;
		Road2_Dir = POS(1, 1) * TILECX;

		Sub_Search1 = RR;
		Sub_Search2 = UU;

		break;

	case RD:
		Dir = POS(1, 1) * TILECX;

		Wall1_Dir = POS(0, -1) * TILECX;
		Road1_Dir = POS(1, -1) * TILECX;
		Wall2_Dir = POS(-1, 0) * TILECX;
		Road2_Dir = POS(-1, 1) * TILECX;

		Sub_Search1 = RR;
		Sub_Search2 = DD;

		break;

	case LD:
		Dir = POS(-1, 1) * TILECX;

		Wall1_Dir = POS(1, 0) * TILECX;
		Road1_Dir = POS(1, 1) * TILECX;
		Wall2_Dir = POS(0, -1) * TILECX;
		Road2_Dir = POS(-1, -1) * TILECX;

		Sub_Search1 = LL;
		Sub_Search2 = DD;
		break;

	case LU:
		Dir = POS(-1, -1) * TILECX;

		Wall1_Dir = POS(0, 1) * TILECX;
		Road1_Dir = POS(-1, 1) * TILECX;
		Wall2_Dir = POS(1, 0) * TILECX;
		Road2_Dir = POS(1, -1) * TILECX;

		Sub_Search1 = LL;
		Sub_Search2 = UU;
		break;

	default:
		break;
	}

	POS CheckPos = _Node.Get_Pos();

	//����
	CheckPos.fX += Dir.fX;
	CheckPos.fY += Dir.fY;

	while (true)
	{
		POS Wall1(CheckPos.fX + Wall1_Dir.fX, CheckPos.fY + Wall1_Dir.fY);
		POS Road1(CheckPos.fX + Road1_Dir.fX, CheckPos.fY + Road1_Dir.fY);
		POS Wall2(CheckPos.fX + Wall2_Dir.fX, CheckPos.fY + Wall2_Dir.fY);
		POS Road2(CheckPos.fX + Road2_Dir.fX, CheckPos.fY + Road2_Dir.fY);

		//���� Ž��

		if (CTileMgr::Get_Instance()->IsValidTile(CheckPos))//���� Ž�� ������ �� �ȿ� �ִ��� Ȯ��
		{
			if (CTileMgr::Get_Instance()->Get_TileOption(CheckPos) != OPT_BLOCKED)//üũ�ϴ� ��ġ�� ������ Ȯ��
			{
				Search_Direction(_Node, Sub_Search1, pOpenList, pCloseList, _tEnd, CheckPos);
				Search_Direction(_Node, Sub_Search2, pOpenList, pCloseList, _tEnd, CheckPos);
			}
		}

		//�� ������ üũ
		if (!CTileMgr::Get_Instance()->IsValidTile(CheckPos))
		{
			break;
		}


		//üũ�ϴ� ��ġ�� ������ ���� Ȯ��
		if (CNode::Distance(_tEnd, CheckPos) < TILECX * 0.5f)//TILCX ���� ������ �����Ѱɷ� ģ��...�³�?
		{
			if (Check_Visit(CheckPos, pOpenList, pCloseList))
			{
				break;
			}

			pOpenList->push_back(new CNode(CheckPos, &_Node, _tEnd, _Dir));

			break;
		}

		//üũ�ϴ� ��ġ�� ������ Ȯ��
		if (CTileMgr::Get_Instance()->Get_TileOption(CheckPos) == OPT_BLOCKED)
		{
			break;
		}

		//���¸���Ʈ �Ǵ� Ŭ���� ����Ʈ�� �̹� �ִ� �������� Ȯ��
		if (Check_Visit(CheckPos, pOpenList, pCloseList))
		{
			break;
		}

		//�ֺ��� �ڳʰ� �ִ��� üũ
		bool bIsNewNode(false);
		if (CTileMgr::Get_Instance()->IsValidTile(Wall1) && CTileMgr::Get_Instance()->IsValidTile(Road1))
		{
			if (CTileMgr::Get_Instance()->Get_TileOption(Wall1) == OPT_BLOCKED && CTileMgr::Get_Instance()->Get_TileOption(Road1) == OPT_REACHABLE)
			{
				pOpenList->push_back(new CNode(CheckPos, &_Node, _tEnd, _Dir));

				bIsNewNode = true;
			}
		}

		if (CTileMgr::Get_Instance()->IsValidTile(Wall2) && CTileMgr::Get_Instance()->IsValidTile(Road2))
		{
			if (CTileMgr::Get_Instance()->Get_TileOption(Wall2) == OPT_BLOCKED && CTileMgr::Get_Instance()->Get_TileOption(Road2) == OPT_REACHABLE)
			{
				pOpenList->push_back(new CNode(CheckPos, &_Node, _tEnd, _Dir));

				bIsNewNode = true;
			}
		}

		//�ڳʰ� ������ ��� ���� �� break;
		if (bIsNewNode)
		{
			break;
		}

		//����
		CheckPos.fX += Dir.fX;
		CheckPos.fY += Dir.fY;
	}


}

bool CPathFinder::Check_Visit(const POS& _Pos, const list<CNode*>* pOpenList, const list<CNode*>* pCloseList)
{
	for (CNode* pNode : *pOpenList)
	{
		if (_Pos == pNode->Get_Pos())
		{
			return true;
		}
	}

	for (CNode* pNode : *pCloseList)
	{
		if (_Pos == pNode->Get_Pos())
		{
			return true;
		}
	}

	return false;
}