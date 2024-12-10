#include "pch.h"
#include "CAnimMgr.h"

CAnimMgr* CAnimMgr::m_pInstance = nullptr;

CAnimMgr::CAnimMgr()
{
}

void CAnimMgr::Update() // �̹��� ������Ʈ ����
{
	//if (false == m_mapAnim.empty())
	//{
	//	for (auto iter = m_mapAnim.begin(); iter != m_mapAnim.end(); ++iter)
	//		iter->second->Update();

	//}
}

void CAnimMgr::Late_Update()
{
	//if (false == m_mapAnim.empty())
	//{
	//	for (auto iter = m_mapAnim.begin(); iter != m_mapAnim.end(); ++iter)
	//	{
	//		if (iter->second->Get)
	//		{
	//			m_mapAnim.erase(iter);
	//			m_mapAnim.insert({ pImgKey, _pAnim });

	//		}
	//		iter->second->Late_Update();
	//	}
	//}
}

void CAnimMgr::Render(HDC _hDc, const TCHAR* _pImgKey, CObj* _pTarget)
{
	auto iter = find_if(m_mapAnim.begin(), m_mapAnim.end(), tagFinder(_pImgKey));
	if (iter != m_mapAnim.end())
		iter->second->Render(_hDc, _pTarget);

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
// ù��° ImgKey = Animation image key / �ι�° ImgKey = BmpMgr���� �̹��� ������ ��ϵǾ��ִ��� ã�� ���� Key
// �ִϸ��̼Ǹ����� Ű(�ȱ�,�޸���)������ ù��° key�� ���, ĳ������ ��ü �̹��� ���� ã�� �뵵�� �ι�° key�� �˻�
void CAnimMgr::Insert_Animation(const TCHAR* pImgKey, CAnimation* _pAnim)
{
	auto	iter = find_if(m_mapAnim.begin(), m_mapAnim.end(), tagFinder(pImgKey));

	if (iter == m_mapAnim.end())
	{
		m_mapAnim.insert({ pImgKey, _pAnim });
	}
	//else if (iter->second != _pAnim)
	//{
	//	m_mapAnim.erase(iter);
	//	m_mapAnim.insert({ pImgKey, _pAnim });

	//}
}
// ã�� �̹��� Ű��, LT��ǥ, �̹��� ũ��(���� ����), ������ ������ �ε��� ��ȣ(0����)
void CAnimMgr::Edit_Animation(const TCHAR* pImgKey, FPOINT _ImgLT, FPOINT _ImgSize, int _FrameIdx)
{
	auto	iter = find_if(m_mapAnim.begin(), m_mapAnim.end(), tagFinder(pImgKey));

	if (iter == m_mapAnim.end())
	{
		return;
	}
	iter->second->Set_Manual_Vector(_ImgLT, _ImgSize, _FrameIdx);
}