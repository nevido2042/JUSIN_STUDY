#include "pch.h"
#include "PathFinder.h"
#include <algorithm>

CPathFinder::CPathFinder():m_pMap(nullptr)
{
}

CPathFinder::~CPathFinder()
{
	Release();
}

void CPathFinder::Initialize(CMap& _pMap)
{
	m_pMap = &_pMap;
	m_StartPos = _pMap.Get_StartPos();
	m_EndPos = _pMap.Get_EndPos();
}

void CPathFinder::Release()
{
	for_each(m_OpenList.begin(), m_OpenList.end(), Safe_Delete<CNode*>);
	for_each(m_CloseList.begin(), m_CloseList.end(), Safe_Delete<CNode*>);

	/*for (int i = 0; i < m_OpenList.size(); ++i)
	{
		Safe_Delete<CNode*>(m_OpenList.top());
	}*/

	/*for (int i = 0; i < m_CloseList.size(); ++i)
	{
		Safe_Delete<CNode*>(m_CloseList.top());
	}*/
}

bool CPathFinder::Is_Position_InMap(POS _Pos)
{
	if (_Pos.iX < 0 || _Pos.iY < 0 || m_pMap->Get_Width() - 1 < _Pos.iX || m_pMap->Get_Height() - 1 < _Pos.iY)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CPathFinder::LessF(const CNode* _First, const CNode* _Second)
{
	return _First->Get_F() < _Second->Get_F();
}

void CPathFinder::Search_Corner(CNode& _Node)
{
	
	switch (_Node.Get_Direction())
	{
	case OO:
		Search_Linear(_Node, UU);
		Search_Linear(_Node, RU);
		Search_Linear(_Node, RR);
		Search_Linear(_Node, RD);
		Search_Linear(_Node, DD);
		Search_Linear(_Node, LD);
		Search_Linear(_Node, LL);
		Search_Linear(_Node, LU);
		break;
	case UU:
		Search_Linear(_Node, UU);
		break;
	case RU:
		Search_Linear(_Node, RU);
		break;
	case RR:
		Search_Linear(_Node, RR);
		break;
	case RD:
		Search_Linear(_Node, RD);
		break;
	case DD:
		Search_Linear(_Node, DD);
		break;
	case LD:
		Search_Linear(_Node, LD);
		break;
	case LL:
		Search_Linear(_Node, LL);
		break;
	case LU:
		Search_Linear(_Node, LU);
		break;
	default:
		break;
	}
}

void CPathFinder::Search_Linear(CNode& _Node, DIRECTION _Dir)
{
	//POS CheckPos = _Node.Get_Pos();
	switch (_Dir)
	{
	case UU:
		Search_Direction(_Node, UU);
		Search_Direction(_Node,	LU);
		Search_Direction(_Node, RU);
		break;

	case RU:
		Search_Direction(_Node, UU);
		Search_Direction(_Node, RU);
		Search_Direction(_Node, RR);

		Search_Direction(_Node, LU);
		Search_Direction(_Node, RD);

		break;
		
	case RR:
		Search_Direction(_Node, RR);
		Search_Direction(_Node, RU);
		Search_Direction(_Node, RD);
		break;

	case RD:
		Search_Direction(_Node, RR);
		Search_Direction(_Node, RD);
		Search_Direction(_Node, DD);
		break;
		
	case DD:
		Search_Direction(_Node, DD);
		Search_Direction(_Node, LD);
		Search_Direction(_Node, RD);
		break;

	case LD:
		Search_Direction(_Node, LL);
		Search_Direction(_Node, LD);
		Search_Direction(_Node, DD);
		break;

	case LL:
		Search_Direction(_Node, LL);
		Search_Direction(_Node, LU);
		Search_Direction(_Node, LD);
		break;

	case LU:
		Search_Direction(_Node, LL);
		Search_Direction(_Node, LU);
		Search_Direction(_Node, UU);
		break;

	default:
		break;
	}
}

void CPathFinder::Search_Direction(const CNode& _Node, DIRECTION _Dir)
{
	POS Dir;
	POS Wall1_Dir;
	POS Road1_Dir;
	POS Wall2_Dir;
	POS Road2_Dir;

	switch (_Dir)
	{
	case UU:
		Dir = POS(0, -1);

		Wall1_Dir = POS(-1, 0);
		Road1_Dir = POS(-1, -1);
		Wall2_Dir = POS(1, 0);
		Road2_Dir = POS(1, -1);
		break;

	case RU:
		Dir = POS(1, -1);

		Wall1_Dir = POS(-1, 0);
		Road1_Dir = POS(-1, -1);
		Wall2_Dir = POS(0, 1);
		Road2_Dir = POS(1, 1);
		break;

	case RR:
		Dir = POS(1, 0);

		Wall1_Dir = POS(0, -1);
		Road1_Dir = POS(1, -1);
		Wall2_Dir = POS(1, 0);
		Road2_Dir = POS(1, -1);
		break;

	case RD:
		Dir = POS(1, 1);

		Wall1_Dir = POS(0, -1);
		Road1_Dir = POS(1, -1);
		Wall2_Dir = POS(-1, 0);
		Road2_Dir = POS(-1, 1);
		break;

	case DD:
		Dir = POS(0, 1);

		Wall1_Dir = POS(1, 0);
		Road1_Dir = POS(1, 1);
		Wall2_Dir = POS(-1, 0);
		Road2_Dir = POS(-1, 1);
		break;

	case LD:
		Dir = POS(-1, 1);

		Wall1_Dir = POS(1, 0);
		Road1_Dir = POS(1, 1);
		Wall2_Dir = POS(0, -1);
		Road2_Dir = POS(-1, -1);
		break;

	case LL:
		Dir = POS(-1, 0);

		Wall1_Dir = POS(0, 1);
		Road1_Dir = POS(-1, 1);
		Wall2_Dir = POS(0, -1);
		Road2_Dir = POS(-1, -1);

		break;

	case LU:
		Dir = POS(-1, -1);

		Wall1_Dir = POS(0, 1);
		Road1_Dir = POS(-1, 1);
		Wall2_Dir = POS(1, 0);
		Road2_Dir = POS(1, -1);

		break;

	default:
		break;
	}

	POS CheckPos = _Node.Get_Pos(); //9,10
	//����
	CheckPos.iX += Dir.iX;
	CheckPos.iY += Dir.iY;

	while (true)
	{
		POS Wall1(CheckPos.iX + Wall1_Dir.iX, CheckPos.iY + Wall1_Dir.iY);
		POS Road1(CheckPos.iX + Road1_Dir.iX, CheckPos.iY + Road1_Dir.iY);
		POS Wall2(CheckPos.iX + Wall2_Dir.iX, CheckPos.iY + Wall2_Dir.iY);
		POS Road2(CheckPos.iX + Road2_Dir.iX, CheckPos.iY + Road2_Dir.iY);

		//�� ������ üũ
		if (!Is_Position_InMap(CheckPos))
		{
			break;
		}

		//üũ�ϴ� ��ġ�� ������ Ȯ��
		if (m_pMap->Get_Tile_Type(CheckPos) == WALL)
		{
			break;
		}

		//�ֺ��� �ڳʰ� �ִ��� üũ

		bool bIsNewNode(false);
		if (Is_Position_InMap(Wall1) && Is_Position_InMap(Road1))
		{
			if (m_pMap->Get_Tile_Type(Wall1) == WALL && m_pMap->Get_Tile_Type(Road1) == ROAD)
			{
				m_OpenList.push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));
				m_pMap->Change_Tile(CheckPos, NODE);
				m_pMap->Update();
				Sleep(50);
				bIsNewNode = true;
			}
		}

		if (Is_Position_InMap(Wall2) && Is_Position_InMap(Road2))
		{
			if (m_pMap->Get_Tile_Type(Wall2) == WALL && m_pMap->Get_Tile_Type(Road2) == ROAD)
			{
				m_OpenList.push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));
				m_pMap->Change_Tile(CheckPos, NODE);
				m_pMap->Update();
				Sleep(50);
				bIsNewNode = true;
			}
		}

		//�ڳʰ� ������ ��� ���� �� break;
		if (bIsNewNode)
		{
			break;
		}

		m_pMap->Change_Tile(CheckPos, CHECK);
		m_pMap->Update();
		Sleep(50);
		
		//����
		CheckPos.iX += Dir.iX;
		CheckPos.iY += Dir.iY;
	}
}

void CPathFinder::Search_Diagonal(CNode& _Node, DIRECTION _Dir)
{
}

void CPathFinder::Start_A_Star()
{
	CNode* StartNode = new CNode(m_StartPos);
	m_OpenList.push_back(StartNode);
	
	while (!m_OpenList.empty())
	{
		//�ϳ� �̴´�.
		CNode* PopNode = m_OpenList.front();
		m_OpenList.pop_front();

		//���� �������� Ȯ���Ѵ�.
		if (m_EndPos == PopNode->Get_Pos())
		{
			m_pMap->Change_Tile(m_EndPos, END);
			// �θ� �̾ ���� �����.
			const CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//���������� ��ĥ���� �ʵ���
				if (pParent->Get_Pos() == m_StartPos)
					break;

				m_pMap->Change_Tile(pParent->Get_Pos(), PATH);
				m_pMap->Update();
				Sleep(10);
			}

			return;
		}

		//Ŭ���� ����Ʈ�� �ִ´�.
		m_CloseList.push_back(PopNode);

		//8���� Ž���Ѵ�.
		int iDX[8]{ 0, 1, 1, 1,	0, -1, -1, -1 };
		int iDY[8]{	-1,	-1, 0, 1, 1, 1,	0, -1 };
				//  12   1  3  4  6   7   9  11

		for (int i = 0; i < 8; ++i)
		{
			int iX = PopNode->Get_Pos().iX + iDX[i];
			int iY = PopNode->Get_Pos().iY + iDY[i];

			//�� ���ΰ�?
			if (iX < 0 || iY < 0 || m_pMap->Get_Width() - 1 < iX || m_pMap->Get_Height() - 1 < iY)
			{
				continue;
			}

			POS NewPos(iX, iY);

			//���ΰ�?
			if (m_pMap->Get_Tile_Type(NewPos) == WALL)
			{
				continue;
			}

			//Ŭ���� ����Ʈ �Ǵ� ���¸���Ʈ�� ���� �ϴ°�?
			bool bIsNewPos = true;
			for (list<CNode*>::iterator iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
			{
				if (NewPos == (*iter)->Get_Pos())
				{
					bIsNewPos = false;
					break;
				}
			}
			for (list<CNode*>::iterator iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
			{
				if (NewPos == (*iter)->Get_Pos())
				{
					bIsNewPos = false;
					break;
				}
			}
			if (bIsNewPos)
			{
				CNode* NewNode = new CNode(NewPos, PopNode, m_EndPos);
				m_OpenList.push_back(NewNode);
				m_pMap->Change_Tile(NewPos, NODE);
			}
		}

		//���� ����Ʈ ����
		m_OpenList.sort(LessF);

		m_pMap->Update();
		Sleep(10);
	}
}

void CPathFinder::Start_JPS()
{
	CNode* StartNode = new CNode(m_StartPos);
	m_OpenList.push_back(StartNode);

	while (!m_OpenList.empty())
	{
		//��带 �ϳ� �̴´�.
		CNode* PopNode = m_OpenList.front();
		m_OpenList.pop_front();
		//���� �����ΰ�?
		if (m_EndPos == PopNode->Get_Pos())
		{
			m_pMap->Change_Tile(m_EndPos, END);
			// �θ� �̾ ���� �����.
			const CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//���������� ��ĥ���� �ʵ���
				if (pParent->Get_Pos() == m_StartPos)
					break;

				m_pMap->Change_Tile(pParent->Get_Pos(), PATH);
				m_pMap->Update();
				Sleep(10);
			}

			return;
		}
		//Ŭ���� ����Ʈ�� �ִ´�.
		m_CloseList.push_back(PopNode);

		//�θ��� �������� Ž��(ù ���� 8��Ž��)
		// - ����Ž��, �밢�� Ž��
		//�ڳʸ� ã�� ���� ����Ʈ�� �ִ´�.
		Search_Corner(*PopNode);
		
		//�����Ѵ�.
		m_OpenList.sort(LessF);

		m_pMap->Update();
		Sleep(10);
	}
}
