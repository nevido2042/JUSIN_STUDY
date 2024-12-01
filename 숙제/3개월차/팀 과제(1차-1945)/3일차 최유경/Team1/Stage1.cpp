#include "pch.h"
#include "Stage1.h"
#include "Monster.h"
#include "BulletMonster.h"

Stage1::Stage1()
{
}

Stage1::~Stage1()
{
}

void Stage1::SpawnMonster()
{
	if (m_dwTime + 700 < GetTickCount64()) {
		m_dwTime = GetTickCount64();
		m_ObjList[OBJ_MONSTER].push_back(new Monster());
		m_ObjList[OBJ_MONSTER].back()->Initialize();
		m_ObjList[OBJ_MONSTER].back()->Set_Target(m_ObjList[OBJ_PLAYER].front());

		m_dwTime = GetTickCount64();
		m_ObjList[OBJ_MONSTER].push_back(new BulletMonster());
		m_ObjList[OBJ_MONSTER].back()->Initialize();
		m_ObjList[OBJ_MONSTER].back()->Set_Target(m_ObjList[OBJ_PLAYER].front());
		if (BulletMonster* pBulletMonster = dynamic_cast<BulletMonster*>(m_ObjList[OBJ_MONSTER].back()))
		{
			pBulletMonster->Set_BulletList(&m_ObjList[OBJ_MONSTERBULLET]);
		}
	}

	//if (m_dwTime + 700 < GetTickCount64()) {

	//}
}
