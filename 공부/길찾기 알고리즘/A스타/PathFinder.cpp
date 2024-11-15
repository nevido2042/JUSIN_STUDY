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
	m_pMap->Change_CHECK_Color();
	
	switch (_Node.Get_Direction())
	{
	case OO:
		Search_Linear(_Node, UU);
		Search_Diagonal(_Node, RU);
		Search_Linear(_Node, RR);
		Search_Diagonal(_Node, RD);
		Search_Linear(_Node, DD);
		Search_Diagonal(_Node, LD);
		Search_Linear(_Node, LL);
		Search_Diagonal(_Node, LU);
		break;
	case UU:
		Search_Linear(_Node, UU);

		Search_Diagonal(_Node, LU);
		Search_Diagonal(_Node, RU);

		break;
	case RU:
		Search_Diagonal(_Node, RU);

		Search_Diagonal(_Node, RD);
		Search_Diagonal(_Node, LU);

		break;
	case RR:
		Search_Linear(_Node, RR);

		Search_Diagonal(_Node, RU);
		Search_Diagonal(_Node, RD);

		break;
	case RD: //ex)
		Search_Diagonal(_Node, RD);

		Search_Diagonal(_Node, RU);
		Search_Diagonal(_Node, LD);

		break;
	case DD:
		Search_Linear(_Node, DD);

		Search_Diagonal(_Node, LD);
		Search_Diagonal(_Node, RD);

		break;
	case LD:
		Search_Diagonal(_Node, LD);

		Search_Diagonal(_Node, LU);
		Search_Diagonal(_Node, RD);
		break;
	case LL:
		Search_Linear(_Node, LL);

		Search_Diagonal(_Node, LD);
		Search_Diagonal(_Node, LU);

		break;
	case LU:
		Search_Diagonal(_Node, LU);

		Search_Diagonal(_Node, RU);
		Search_Diagonal(_Node, LD);
		break;
	default:
		break;
	}
}

void CPathFinder::Search_Linear(CNode& _Node, DIRECTION _Dir)
{
	switch (_Dir)
	{
	case UU:
		Search_Direction(_Node, UU);

		Search_Diagonal(_Node, RU);
		Search_Diagonal(_Node, LU);
		break;
		
	case RR:
		Search_Direction(_Node, RR);

		Search_Diagonal(_Node, RU);
		Search_Diagonal(_Node, RD);
		break;
		
	case DD:
		Search_Direction(_Node, DD);

		Search_Diagonal(_Node, RD);
		Search_Diagonal(_Node, LD);
		break;

	case LL:
		Search_Direction(_Node, LL);

		Search_Diagonal(_Node, LD);
		Search_Diagonal(_Node, LU);
		break;

	default:
		break;
	}
}

void CPathFinder::Search_Direction(const CNode& _Node, DIRECTION _Dir, const POS _Pos)
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

	//case RU:
	//	Dir = POS(1, -1);

	//	Wall1_Dir = POS(-1, 0);
	//	Road1_Dir = POS(-1, -1);
	//	Wall2_Dir = POS(0, 1);
	//	Road2_Dir = POS(1, 1);
	//	break;

	case RR:
		Dir = POS(1, 0);

		Wall1_Dir = POS(0, -1);
		Road1_Dir = POS(1, -1);
		Wall2_Dir = POS(1, 0);
		Road2_Dir = POS(1, -1);
		break;

	//case RD:
	//	Dir = POS(1, 1);

	//	Wall1_Dir = POS(0, -1);
	//	Road1_Dir = POS(1, -1);
	//	Wall2_Dir = POS(-1, 0);
	//	Road2_Dir = POS(-1, 1);
	//	break;

	case DD:
		Dir = POS(0, 1);

		Wall1_Dir = POS(1, 0);
		Road1_Dir = POS(1, 1);
		Wall2_Dir = POS(-1, 0);
		Road2_Dir = POS(-1, 1);
		break;

	//case LD:
	//	Dir = POS(-1, 1);

	//	Wall1_Dir = POS(1, 0);
	//	Road1_Dir = POS(1, 1);
	//	Wall2_Dir = POS(0, -1);
	//	Road2_Dir = POS(-1, -1);
	//	break;

	case LL:
		Dir = POS(-1, 0);

		Wall1_Dir = POS(0, 1);
		Road1_Dir = POS(-1, 1);
		Wall2_Dir = POS(0, -1);
		Road2_Dir = POS(-1, -1);

		break;

	//case LU:
	//	Dir = POS(-1, -1);

	//	Wall1_Dir = POS(0, 1);
	//	Road1_Dir = POS(-1, 1);
	//	Wall2_Dir = POS(1, 0);
	//	Road2_Dir = POS(1, -1);

	//	break;

	default:
		break;
	}

	POS CheckPos;
	if (_Pos == POS(-999, -999))
	{
		CheckPos = _Node.Get_Pos(); //9,SLEEP_TIME
	}
	else
	{
		CheckPos = _Pos;
	}

	//전진
	CheckPos.iX += Dir.iX;
	CheckPos.iY += Dir.iY;

	while (true)
	{
		POS Wall1(CheckPos.iX + Wall1_Dir.iX, CheckPos.iY + Wall1_Dir.iY);
		POS Road1(CheckPos.iX + Road1_Dir.iX, CheckPos.iY + Road1_Dir.iY);
		POS Wall2(CheckPos.iX + Wall2_Dir.iX, CheckPos.iY + Wall2_Dir.iY);
		POS Road2(CheckPos.iX + Road2_Dir.iX, CheckPos.iY + Road2_Dir.iY);

		//맵 밖인지 체크
		if (!Is_Position_InMap(CheckPos))
		{
			break;
		}

		//체크하는 위치가 도착점 인지 확인
		if (m_pMap->Get_Tile_Type(CheckPos) == END)
		{
			m_OpenList.push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));
			//m_pMap->Change_Tile(CheckPos, NODE);
			//m_pMap->Update();
			Sleep(SLEEP_TIME);
			//bIsNewNode = true;
			break;
		}

		//체크하는 위치가 벽인지 확인
		if (m_pMap->Get_Tile_Type(CheckPos) == WALL)
		{
			break;
		}

		//오픈리스트 또는 클로즈 리스트에 이미 있는 지점인지 확인
		bool bIsNewPos = true;
		for (list<CNode*>::iterator iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
		{
			if (CheckPos == (*iter)->Get_Pos())
			{
				bIsNewPos = false;
				break;
			}
		}
		for (list<CNode*>::iterator iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
		{
			if (CheckPos == (*iter)->Get_Pos())
			{
				bIsNewPos = false;
				break;
			}
		}
		if (!bIsNewPos)
		{
			break;
		}

		//주변에 코너가 있는지 체크
		bool bIsNewNode(false);
		if (Is_Position_InMap(Wall1) && Is_Position_InMap(Road1))
		{
			if (m_pMap->Get_Tile_Type(Wall1) == WALL && m_pMap->Get_Tile_Type(Road1) == ROAD)
			{
				m_OpenList.push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));
				m_pMap->Change_Tile(CheckPos, NODE);
				m_pMap->Update();
				Sleep(SLEEP_TIME);
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
				Sleep(SLEEP_TIME);
				bIsNewNode = true;
			}
		}

		//코너가 있으면 노드 생성 후 break;
		if (bIsNewNode)
		{
			break;
		}

		m_pMap->Change_Tile(CheckPos, CHECK);
		m_pMap->Update();
		Sleep(SLEEP_TIME);
		
		//전진
		CheckPos.iX += Dir.iX;
		CheckPos.iY += Dir.iY;
	}
}

void CPathFinder::Search_Diagonal(CNode& _Node, DIRECTION _Dir)
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
	//case UU:
	//	Dir = POS(0, -1);

	//	Wall1_Dir = POS(-1, 0);
	//	Road1_Dir = POS(-1, -1);
	//	Wall2_Dir = POS(1, 0);
	//	Road2_Dir = POS(1, -1);
	//	break;

	case RU:
		Dir = POS(1, -1);

		Wall1_Dir = POS(-1, 0);
		Road1_Dir = POS(-1, -1);
		Wall2_Dir = POS(0, 1);
		Road2_Dir = POS(1, 1);

		Sub_Search1 = RR;
		Sub_Search2 = UU;

		break;

	//case RR:
	//	Dir = POS(1, 0);

	//	Wall1_Dir = POS(0, -1);
	//	Road1_Dir = POS(1, -1);
	//	Wall2_Dir = POS(1, 0);
	//	Road2_Dir = POS(1, -1);
	//	break;

	case RD:
		Dir = POS(1, 1);

		Wall1_Dir = POS(0, -1);
		Road1_Dir = POS(1, -1);
		Wall2_Dir = POS(-1, 0);
		Road2_Dir = POS(-1, 1);

		Sub_Search1 = RR;
		Sub_Search2 = DD;

		break;

	//case DD:
	//	Dir = POS(0, 1);

	//	Wall1_Dir = POS(1, 0);
	//	Road1_Dir = POS(1, 1);
	//	Wall2_Dir = POS(-1, 0);
	//	Road2_Dir = POS(-1, 1);
	//	break;

	case LD:
		Dir = POS(-1, 1);

		Wall1_Dir = POS(1, 0);
		Road1_Dir = POS(1, 1);
		Wall2_Dir = POS(0, -1);
		Road2_Dir = POS(-1, -1);

		Sub_Search1 = LL;
		Sub_Search2 = DD;
		break;

	//case LL:
	//	Dir = POS(-1, 0);

	//	Wall1_Dir = POS(0, 1);
	//	Road1_Dir = POS(-1, 1);
	//	Wall2_Dir = POS(0, -1);
	//	Road2_Dir = POS(-1, -1);

	//	break;

	case LU:
		Dir = POS(-1, -1);

		Wall1_Dir = POS(0, 1);
		Road1_Dir = POS(-1, 1);
		Wall2_Dir = POS(1, 0);
		Road2_Dir = POS(1, -1);

		Sub_Search1 = LL;
		Sub_Search2 = UU;
		break;

	default:
		break;
	}

	POS CheckPos = _Node.Get_Pos(); //9,SLEEP_TIME
	//전진
	CheckPos.iX += Dir.iX;
	CheckPos.iY += Dir.iY;

	while (true)
	{
		POS Wall1(CheckPos.iX + Wall1_Dir.iX, CheckPos.iY + Wall1_Dir.iY);
		POS Road1(CheckPos.iX + Road1_Dir.iX, CheckPos.iY + Road1_Dir.iY);
		POS Wall2(CheckPos.iX + Wall2_Dir.iX, CheckPos.iY + Wall2_Dir.iY);
		POS Road2(CheckPos.iX + Road2_Dir.iX, CheckPos.iY + Road2_Dir.iY);

		//보조 탐색

		if(Is_Position_InMap(CheckPos))//보조 탐색 범위가 맵 안에 있는지 확인
		{
			if (m_pMap->Get_Tile_Type(CheckPos) != WALL)//체크하는 위치가 벽인지 확인
			{
				Search_Direction(_Node, Sub_Search1, CheckPos);
				Search_Direction(_Node, Sub_Search2, CheckPos);
			}
		}

		//맵 밖인지 체크
		if (!Is_Position_InMap(CheckPos))
		{
			break;
		}

		//체크하는 위치가 도착점 인지 확인
		if (m_pMap->Get_Tile_Type(CheckPos) == END)
		{
			m_OpenList.push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));
			//m_pMap->Change_Tile(CheckPos, NODE);
			//m_pMap->Update();
			Sleep(SLEEP_TIME);
			//bIsNewNode = true;
			break;
		}

		//체크하는 위치가 벽인지 확인
		if (m_pMap->Get_Tile_Type(CheckPos) == WALL)
		{
			break;
		}

		//오픈리스트 또는 클로즈 리스트에 이미 있는 지점인지 확인
		bool bIsNewPos = true;
		for (list<CNode*>::iterator iter = m_OpenList.begin(); iter != m_OpenList.end(); ++iter)
		{
			if (CheckPos == (*iter)->Get_Pos())
			{
				bIsNewPos = false;
				break;
			}
		}
		for (list<CNode*>::iterator iter = m_CloseList.begin(); iter != m_CloseList.end(); ++iter)
		{
			if (CheckPos == (*iter)->Get_Pos())
			{
				bIsNewPos = false;
				break;
			}
		}
		if (!bIsNewPos)
		{
			break;
		}

		//주변에 코너가 있는지 체크
		bool bIsNewNode(false);
		if (Is_Position_InMap(Wall1) && Is_Position_InMap(Road1))
		{
			if (m_pMap->Get_Tile_Type(Wall1) == WALL && m_pMap->Get_Tile_Type(Road1) == ROAD)
			{
				m_OpenList.push_back(new CNode(CheckPos, &_Node, m_EndPos, _Dir));
				m_pMap->Change_Tile(CheckPos, NODE);
				m_pMap->Update();
				Sleep(SLEEP_TIME);
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
				Sleep(SLEEP_TIME);
				bIsNewNode = true;
			}
		}

		//코너가 있으면 노드 생성 후 break;
		if (bIsNewNode)
		{
			break;
		}

		m_pMap->Change_Tile(CheckPos, CHECK);
		m_pMap->Update();
		Sleep(SLEEP_TIME);

		//전진
		CheckPos.iX += Dir.iX;
		CheckPos.iY += Dir.iY;
	}


}

void CPathFinder::Start_A_Star()
{
	CNode* StartNode = new CNode(m_StartPos);
	m_OpenList.push_back(StartNode);
	
	while (!m_OpenList.empty())
	{
		//하나 뽑는다.
		CNode* PopNode = m_OpenList.front();
		m_OpenList.pop_front();

		//도착 지점인지 확인한다.
		if (m_EndPos == PopNode->Get_Pos())
		{
			m_pMap->Change_Tile(m_EndPos, END);
			// 부모를 이어서 길을 만든다.
			const CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//시작지점은 색칠하지 않도록
				if (pParent->Get_Pos() == m_StartPos)
					break;

				m_pMap->Change_Tile(pParent->Get_Pos(), PATH);
				m_pMap->Update();
				Sleep(SLEEP_TIME);
			}

			return;
		}

		//클로즈 리스트에 넣는다.
		m_CloseList.push_back(PopNode);

		//8방향 탐색한다.
		int iDX[8]{ 0, 1, 1, 1,	0, -1, -1, -1 };
		int iDY[8]{	-1,	-1, 0, 1, 1, 1,	0, -1 };
				//  12   1  3  4  6   7   9  11

		for (int i = 0; i < 8; ++i)
		{
			int iX = PopNode->Get_Pos().iX + iDX[i];
			int iY = PopNode->Get_Pos().iY + iDY[i];

			//맵 밖인가?
			if (iX < 0 || iY < 0 || m_pMap->Get_Width() - 1 < iX || m_pMap->Get_Height() - 1 < iY)
			{
				continue;
			}

			POS NewPos(iX, iY);

			//벽인가?
			if (m_pMap->Get_Tile_Type(NewPos) == WALL)
			{
				continue;
			}

			//클로즈 리스트 또는 오픈리스트에 존재 하는가?
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

		//오픈 리스트 정렬
		m_OpenList.sort(LessF);

		m_pMap->Update();
		Sleep(SLEEP_TIME);
	}
}

void CPathFinder::Start_JPS()
{
	CNode* StartNode = new CNode(m_StartPos);
	m_OpenList.push_back(StartNode);

	while (!m_OpenList.empty())
	{
		//노드를 하나 뽑는다.
		CNode* PopNode = m_OpenList.front();
		m_OpenList.pop_front();
		//도착 지점인가?
		if (m_EndPos == PopNode->Get_Pos())
		{
			m_pMap->Change_Tile(m_EndPos, END);
			// 부모를 이어서 길을 만든다.
			const CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//시작지점은 색칠하지 않도록
				if (pParent->Get_Pos() == m_StartPos)
					break;

				m_pMap->Change_Tile(pParent->Get_Pos(), PATH);
				m_pMap->Update();
				Sleep(SLEEP_TIME);
			}

			return;
		}
		//클로즈 리스트에 넣는다.
		m_CloseList.push_back(PopNode);

		//부모의 방향으로 탐색(첫 노드는 8방탐색)
		// - 직진탐색, 대각선 탐색
		//코너를 찾아 오픈 리스트에 넣는다.
		Search_Corner(*PopNode);
		
		//정렬한다.
		m_OpenList.sort(LessF);

		m_pMap->Update();
		Sleep(SLEEP_TIME);
	}
}
