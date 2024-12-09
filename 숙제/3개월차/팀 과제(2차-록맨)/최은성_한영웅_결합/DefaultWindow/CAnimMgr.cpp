#include "pch.h"
#include "CAnimMgr.h"

CAnimMgr* CAnimMgr::m_pInstance = nullptr;

CAnimMgr::CAnimMgr()
{
}

void CAnimMgr::Update()
{
	if (false == m_mapAnim.empty())
	{
		for (auto iter = m_mapAnim.begin(); iter != m_mapAnim.end(); ++iter)
			iter->second->Update();
	}
}

void CAnimMgr::Late_Update()
{
	if (false == m_mapAnim.empty())
	{
		for (auto iter = m_mapAnim.begin(); iter != m_mapAnim.end(); ++iter)
			iter->second->Late_Update();
	}
}

void CAnimMgr::Render(HDC _hDc, const TCHAR* _pImgKey)
{
	auto iter = find_if(m_mapAnim.begin(), m_mapAnim.end(), tagFinder(_pImgKey));
	if (iter != m_mapAnim.end())
		iter->second->Render(_hDc);

	//if (false == m_mapAnim.empty())
	//{
	//	for (auto iter = m_mapAnim.begin(); iter != m_mapAnim.end(); ++iter)
	//		iter->second->Render(_hDc);
	//}
}

void CAnimMgr::Release()
{
	for_each(m_mapAnim.begin(), m_mapAnim.end(), DeleteMap());
	m_mapAnim.clear();
}

void CAnimMgr::Insert_Animation(const TCHAR* pImgKey, CAnimation* _pAnim)
{
	auto	iter = find_if(m_mapAnim.begin(), m_mapAnim.end(), tagFinder(pImgKey));

	if (iter == m_mapAnim.end())
	{
		m_mapAnim.insert({ pImgKey, _pAnim });
	}
}
