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

    // 웨이브에 맞는 소환 함수 배열
    void (CColony::*WaveFuncs[3])();
    int     m_iWaveIndex;

    //bool    m_bEnemySpawned;
    float   m_fSpawnTime;

    //bool    m_bShipBtnActive;
};

template<typename T>
inline void CColony::Spawn_Random()
{
    //동서남북 랜덤
    int iX = rand() % TILEX; // X 좌표를 0에서 TILEX - 1 사이로 설정
    int iY = rand() % TILEY; // Y 좌표를 0에서 TILEY - 1 사이로 설정

    // 0부터 3까지의 숫자 중 하나를 랜덤하게 선택
    int iDir = rand() % 4;

    switch (iDir) {
    case 0: // 북쪽 (North)
        iY = 0;
        break;
    case 1: // 동쪽 (East)
        iX = TILEX - 1;
        break;
    case 2: // 남쪽 (South)
        iY = TILEY - 1;
        break;
    case 3: // 서쪽 (West)
        iX = 0;
        break;
    }


    POS tPos{ 64 * iX + 32, 64 * iY + 32 };

    CObj* pObj = CAbstractFactory<T>::Create(tPos);
    CObjMgr::Get_Instance()->Add_Object(OBJ_ENEMY, pObj);

}
