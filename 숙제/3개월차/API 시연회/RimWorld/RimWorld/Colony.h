#pragma once
#include "Scene.h"

#include "ColonyMgr.h"
#include "Tile.h"
#include "Rim.h"
#include "SteelWall.h"
#include "Centipede.h"
#include "Lancer.h"
#include "Tree.h"
#include "Boomrat.h"
#include "MyButton.h"
#include "GameSpeedBtn.h"
#include "RimSlotMgr.h"
#include "DetailView.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Panel.h"

class CColony :
    public CScene
{
public:
    CColony();
    virtual ~CColony();
//public:
//    bool    Get_ShipBtnActive() { return m_bShipBtnActive; }
//    void    Set_ShipBtnActive(bool _b) { m_bShipBtnActive = _b; }
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Create_UI();
    void Create_ShipBtn();

    void Spawn_Wave1();
    void Spawn_Wave2();
    void Spawn_Wave3();

    template<typename T>
    void Spawn_Random();

    // ���̺꿡 �´� ��ȯ �Լ� �迭
    void (CColony::*WaveFuncs[3])();
    int     m_iWaveIndex;

    //bool    m_bEnemySpawned;
    float   m_fSpawnTime;

    //bool    m_bShipBtnActive;
};

template<typename T>
inline void CColony::Spawn_Random()
{
    //�������� ����
    int iX = rand() % TILEX; // X ��ǥ�� 0���� TILEX - 1 ���̷� ����
    int iY = rand() % TILEY; // Y ��ǥ�� 0���� TILEY - 1 ���̷� ����

    // 0���� 3������ ���� �� �ϳ��� �����ϰ� ����
    int iDir = rand() % 4;

    switch (iDir) {
    case 0: // ���� (North)
        iY = 0;
        break;
    case 1: // ���� (East)
        iX = TILEX - 1;
        break;
    case 2: // ���� (South)
        iY = TILEY - 1;
        break;
    case 3: // ���� (West)
        iX = 0;
        break;
    }


    POS tPos{ 64 * iX + 32, 64 * iY + 32 };

    CObj* pObj = CAbstractFactory<T>::Create(tPos);
    CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, pObj);

}
