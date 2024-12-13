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

			Safe_Delete(PopNode);
			return;
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

void CPathFinder::Search_Corner(const CNode& _Node, const list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEndPos)
{
	switch (_Node.Get_Direction())
	{
	case OO:
		Search_Linear(_Node, UU, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, RU, pOpenList);
		Search_Linear(_Node, RR, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, RD, pOpenList);
		Search_Linear(_Node, DD, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LD, pOpenList);
		Search_Linear(_Node, LL, pOpenList, pCloseList, _tEndPos);
		Search_Diagonal(_Node, LU, pOpenList);
		break;
	case UU:
		Search_Linear(_Node, UU, pOpenList, pCloseList, _tEndPos);

		break;
	case RU:
		Search_Diagonal(_Node, RU, pOpenList);

		Search_Diagonal(_Node, RD, pOpenList);
		Search_Diagonal(_Node, LU, pOpenList);

		break;
	case RR:
		Search_Linear(_Node, RR, pOpenList, pCloseList, _tEndPos);

		break;
	case RD:
		Search_Diagonal(_Node, RD, pOpenList);

		Search_Diagonal(_Node, RU, pOpenList);
		Search_Diagonal(_Node, LD, pOpenList);

		break;
	case DD:
		Search_Linear(_Node, DD, pOpenList, pCloseList, _tEndPos);

		break;
	case LD:
		Search_Diagonal(_Node, LD, pOpenList);

		Search_Diagonal(_Node, LU, pOpenList);
		Search_Diagonal(_Node, RD, pOpenList);
		break;
	case LL:
		Search_Linear(_Node, LL, pOpenList, pCloseList, _tEndPos);

		break;
	case LU:
		Search_Diagonal(_Node, LU, pOpenList);

		Search_Diagonal(_Node, RU, pOpenList);
		Search_Diagonal(_Node, LD, pOpenList);
		break;
	default:
		break;
	}
}

void CPathFinder::Search_Linear(const CNode& _Node, const DIRECTION _Dir, const list<CNode*>* pOpenList, const list<CNode*>* pCloseList, const POS _tEndPos)
{
	switch (_Dir)
	{
	case UU:
		Search_Direction(_Node, UU, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RU, pOpenList);
		Search_Diagonal(_Node, LU, pOpenList);
		break;

	case RR:
		Search_Direction(_Node, RR, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RU, pOpenList);
		Search_Diagonal(_Node, RD, pOpenList);
		break;

	case DD:
		Search_Direction(_Node, DD, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, RD, pOpenList);
		Search_Diagonal(_Node, LD, pOpenList);
		break;

	case LL:
		Search_Direction(_Node, LL, pOpenList, pCloseList, _tEndPos);

		Search_Diagonal(_Node, LD, pOpenList);
		Search_Diagonal(_Node, LU, pOpenList);
		break;

	default:
		break;
	}
}

void CPathFinder::Search_Direction(const CNode& _Node, const DIRECTION _Dir, const list<CNode*>* pOpenList,  const list<CNode*>* pCloseList,  const POS _tEnd, const POS _Pos)
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
		if (CheckPos.fX < 0 || CheckPos.fY < 0 || TILECX * TILEX < CheckPos.fX || TILECY * TILEY < CheckPos.fY)
		{
			continue;
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

			pOpenList->push_back(new CNode(New_Node_Pos, &_Node, m_EndPos, _Dir));

			break;
		}

		//üũ�ϴ� ��ġ�� ������ Ȯ��
		if (m_pMap->Get_Tile_Type(CheckPos) == WALL)
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

		//���¸���Ʈ �Ǵ� Ŭ���� ����Ʈ�� �̹� �ִ� �������� Ȯ��
		if (Check_Visit(New_Node_Pos))
		{
			break;
		}

		//�ֺ��� �ڳʰ� �ִ��� üũ
		bool bIsNewNode(false);
		if (Is_Position_InMap(Wall1) && Is_Position_InMap(Road1))
		{
			if (m_pMap->Get_Tile_Type(Wall1) == WALL && m_pMap->Get_Tile_Type(Road1) == ROAD)
			{
				pOpenList->push_back(new CNode(New_Node_Pos, &_Node, m_EndPos, _Dir));

				bIsNewNode = true;
			}
		}

		if (Is_Position_InMap(Wall2) && Is_Position_InMap(Road2))
		{
			if (m_pMap->Get_Tile_Type(Wall2) == WALL && m_pMap->Get_Tile_Type(Road2) == ROAD)
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

				pOpenList->push_back(new CNode(New_Node_Pos, &_Node, m_EndPos, _Dir));

				bIsNewNode = true;
			}
		}

		//�ڳʰ� ������ ��� ���� �� break;
		if (bIsNewNode)
		{
			break;
		}

		m_pMap->Change_Tile_Check(CheckPos);

		//����
		CheckPos.fX += Dir.fX;
		CheckPos.fY += Dir.fY;
	}
}

void CPathFinder::Search_Diagonal(const CNode& _Node, const DIRECTION _Dir, const list<CNode*>* pOpenList)
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

		if (Is_Position_InMap(CheckPos))//���� Ž�� ������ �� �ȿ� �ִ��� Ȯ��
		{
			if (m_pMap->Get_Tile_Type(CheckPos) != WALL)//üũ�ϴ� ��ġ�� ������ Ȯ��
			{
				Search_Direction(_Node, Sub_Search1, CheckPos);
				Search_Direction(_Node, Sub_Search2, CheckPos);
			}
		}

		//�� ������ üũ
		if (!Is_Position_InMap(CheckPos))
		{
			break;
		}

		//üũ�ϴ� ��ġ�� ������ ���� Ȯ��
		if (m_pMap->Get_Tile_Type(CheckPos) == END)
		{
			if (Check_Visit(CheckPos))
			{
				break;
			}

			pOpenList->push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));

			break;
		}

		//üũ�ϴ� ��ġ�� ������ Ȯ��
		if (m_pMap->Get_Tile_Type(CheckPos) == WALL)
		{
			break;
		}

		//���¸���Ʈ �Ǵ� Ŭ���� ����Ʈ�� �̹� �ִ� �������� Ȯ��
		if (Check_Visit(CheckPos))
		{
			break;
		}

		//�ֺ��� �ڳʰ� �ִ��� üũ
		bool bIsNewNode(false);
		if (Is_Position_InMap(Wall1) && Is_Position_InMap(Road1))
		{
			if (m_pMap->Get_Tile_Type(Wall1) == WALL && m_pMap->Get_Tile_Type(Road1) == ROAD)
			{
				pOpenList->push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));

				bIsNewNode = true;
			}
		}

		if (Is_Position_InMap(Wall2) && Is_Position_InMap(Road2))
		{
			if (m_pMap->Get_Tile_Type(Wall2) == WALL && m_pMap->Get_Tile_Type(Road2) == ROAD)
			{
				pOpenList->push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));

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