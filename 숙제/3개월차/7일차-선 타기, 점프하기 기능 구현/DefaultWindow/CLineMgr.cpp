#include "pch.h"
#include "CLineMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}

CLineMgr::~CLineMgr()
{
	Release();
}

bool CLineMgr::Collision_Line(INFO* _pInfo, float* pY)
{
	float fX = _pInfo->fX;
	float fY = _pInfo->fY;

	if(m_Linelist.empty())
		return false;


	CLine* pTargetLine = nullptr;
	list<CLine*> TargetLineList;
	for (auto& pLine : m_Linelist)
	{
		if (fX >= pLine->Get_Info().tLPoint.fX &&
			fX < pLine->Get_Info().tRPoint.fX)
		{
			TargetLineList.push_back(pLine);
			//pTargetLine = pLine;
		}
	}
	//x좌표로 걸러진 라인들 중, 내 발 아래 있고, 그 중에 가장 높은 녀석을 선택한다.

	float fHeight(0.f);
	for (auto& pLine : TargetLineList)
	{
		if (pLine->Get_Info().tLPoint.fY < fY&&
			pLine->Get_Info().tRPoint.fY < fY)
		{
			continue;
		}

		//if (_fY > pLine->Get_Info().tLPoint.fY)
		//{
		//	continue;
		//	//return false;
		//}

		if (!pTargetLine || pLine->Get_Info().tLPoint.fY < fHeight)
		{
			pTargetLine = pLine;
			fHeight = pLine->Get_Info().tLPoint.fY;
		}
	}


	if (!pTargetLine)
		return false;

	float	x1 = pTargetLine->Get_Info().tLPoint.fX;
	float	y1 = pTargetLine->Get_Info().tLPoint.fY;
	float	x2 = pTargetLine->Get_Info().tRPoint.fX;
	float	y2 = pTargetLine->Get_Info().tRPoint.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (fX - x1) + y1;
	*pY -= _pInfo->fCY * 0.5f;

	/*if (pTargetLine->Get_Info().tLPoint.fY > _pInfo->fY)
	{
		return false;
	}
	else
	{
		return true;
	}*/

	return true;

}

void CLineMgr::Initialize()
{
	LINEPOINT	tLinePoint[6]{

		{ 100.f, 450.f },
		{ 300.f, 450.f },
		{ 500.f, 250.f },
		{ 700.f, 250.f },

		{ 100.f, 300.f },
		{ 300.f, 300.f }
	};

	m_Linelist.push_back(new CLine(tLinePoint[4], tLinePoint[5]));

	m_Linelist.push_back(new CLine(tLinePoint[0], tLinePoint[1]));
	m_Linelist.push_back(new CLine(tLinePoint[1], tLinePoint[2]));
	m_Linelist.push_back(new CLine(tLinePoint[2], tLinePoint[3]));
}

void CLineMgr::Render(HDC hDC)
{
	for (auto& pLine : m_Linelist)
		pLine->Render(hDC);
}

void CLineMgr::Release()
{
	for_each(m_Linelist.begin(), m_Linelist.end(), Safe_Delete<CLine*>);
	m_Linelist.clear();
}
