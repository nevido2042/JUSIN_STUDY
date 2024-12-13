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
		//노드를 하나 뽑는다.
		CNode* PopNode = OpenList.front();
		OpenList.pop_front();

		//클로즈 리스트에 넣는다.
		CloseList.push_back(PopNode);

		//도착 지점인가?
		if (_tEnd == PopNode->Get_Pos())
		{
			// 부모를 이어서 길을 만든다.
			const CNode* pParent = PopNode;
			while (pParent = pParent->Get_Parent())
			{
				//시작지점은 색칠하지 않도록
				if (pParent->Get_Pos() == _tStart)
					break;
			}

			for_each(OpenList.begin(), OpenList.end(), Safe_Delete<CNode*>);
			OpenList.clear();

			return CloseList;
		}

		//부모의 방향으로 탐색(첫 노드는 8방탐색)
		// - 직진탐색, 대각선 탐색
		//코너를 찾아 오픈 리스트에 넣는다.
		Search_Corner(*PopNode, &OpenList, &CloseList, _tEnd);

		//정렬한다.
		OpenList.sort(LessF);
	}

	for_each(OpenList.begin(), OpenList.end(), Safe_Delete<CNode*>);
	OpenList.clear();

	for_each(CloseList.begin(), CloseList.end(), Safe_Delete<CNode*>);
	CloseList.clear();

	return CloseList;//실패
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

	//전진
	CheckPos.fX += Dir.fX;
	CheckPos.fY += Dir.fY;

	while (true)
	{
		POS Wall1(CheckPos.fX + Wall1_Dir.fX, CheckPos.fY + Wall1_Dir.fY);
		POS Road1(CheckPos.fX + Road1_Dir.fX, CheckPos.fY + Road1_Dir.fY);
		POS Wall2(CheckPos.fX + Wall2_Dir.fX, CheckPos.fY + Wall2_Dir.fY);
		POS Road2(CheckPos.fX + Road2_Dir.fX, CheckPos.fY + Road2_Dir.fY);

		//맵 밖인가?
		if (!CTileMgr::Get_Instance()->IsValidTile(CheckPos))
		{
			break;
		}

		//체크하는 위치가 도착점 인지 확인
		if (CNode::Distance(_tEnd, CheckPos) < TILECX * 0.5f)//TILCX 보다 가까우면 도착한걸로 친다...맞나?
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

		//fx fy로 인덱스 계산
		int iIdx = int(CheckPos.fX / TILECX) + int(CheckPos.fY / TILECY) * TILEX;

		//if (TILEX * TILEY <= iIdx)
		//{
		//	continue;
		//	//가끔 타일 벗어난 값이들어옴 왜지? ex 600 611
		//}

		//벽인가?
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

		//오픈리스트 또는 클로즈 리스트에 이미 있는 지점인지 확인
		if (Check_Visit(New_Node_Pos, pOpenList, pCloseList))
		{
			break;
		}

		//주변에 코너가 있는지 체크
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

		//코너가 있으면 노드 생성 후 break;
		if (bIsNewNode)
		{
			break;
		}

		//전진
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

	//전진
	CheckPos.fX += Dir.fX;
	CheckPos.fY += Dir.fY;

	while (true)
	{
		POS Wall1(CheckPos.fX + Wall1_Dir.fX, CheckPos.fY + Wall1_Dir.fY);
		POS Road1(CheckPos.fX + Road1_Dir.fX, CheckPos.fY + Road1_Dir.fY);
		POS Wall2(CheckPos.fX + Wall2_Dir.fX, CheckPos.fY + Wall2_Dir.fY);
		POS Road2(CheckPos.fX + Road2_Dir.fX, CheckPos.fY + Road2_Dir.fY);

		//보조 탐색

		if (CTileMgr::Get_Instance()->IsValidTile(CheckPos))//보조 탐색 범위가 맵 안에 있는지 확인
		{
			if (CTileMgr::Get_Instance()->Get_TileOption(CheckPos) != OPT_BLOCKED)//체크하는 위치가 벽인지 확인
			{
				Search_Direction(_Node, Sub_Search1, pOpenList, pCloseList, _tEnd, CheckPos);
				Search_Direction(_Node, Sub_Search2, pOpenList, pCloseList, _tEnd, CheckPos);
			}
		}

		//맵 밖인지 체크
		if (!CTileMgr::Get_Instance()->IsValidTile(CheckPos))
		{
			break;
		}


		//체크하는 위치가 도착점 인지 확인
		if (CNode::Distance(_tEnd, CheckPos) < TILECX * 0.5f)//TILCX 보다 가까우면 도착한걸로 친다...맞나?
		{
			if (Check_Visit(CheckPos, pOpenList, pCloseList))
			{
				break;
			}

			pOpenList->push_back(new CNode(CheckPos, &_Node, _tEnd, _Dir));

			break;
		}

		//체크하는 위치가 벽인지 확인
		if (CTileMgr::Get_Instance()->Get_TileOption(CheckPos) == OPT_BLOCKED)
		{
			break;
		}

		//오픈리스트 또는 클로즈 리스트에 이미 있는 지점인지 확인
		if (Check_Visit(CheckPos, pOpenList, pCloseList))
		{
			break;
		}

		//주변에 코너가 있는지 체크
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

		//코너가 있으면 노드 생성 후 break;
		if (bIsNewNode)
		{
			break;
		}

		//전진
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