#pragma once
#include "CObj.h"
class CSatellite :
    public CObj
{
public:
    CSatellite();
    virtual ~CSatellite();
public:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
public:
    void Set_Player(CObj* _pPlayer) { m_pPlayer = _pPlayer; }
    void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }

private:
    CObj* m_pPlayer;
    float m_fDistance;
    float m_fAngle;
};

