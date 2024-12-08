#pragma once
#include "Define.h"
#include "CScene.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();
public:

	static CSceneMgr* Get_Instance() // 싱글톤 특유의 최초 한번만 생성하는 기능
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}
	static void	Destroy_Instance() // 객체 삭제
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
	void Set_Scene(SCENE_TYPE eType);  // 씬 전환
	void Initialize(); // 초기화
	void Update(); // 업데이트
	void LateUpdate();
	void Render(HDC hDC); // 렌더

private:
	static CSceneMgr* m_pInstance;  // 객체 담을 주소
	CScene* m_arrScene[(UINT)SCENE_TYPE::SCENE_END]; // 모든씬
	CScene* m_pCurScene; // 현재씬




};

