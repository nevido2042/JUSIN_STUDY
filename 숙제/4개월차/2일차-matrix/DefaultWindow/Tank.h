#pragma once
#include "CObj.h"
class CTank :
    public CObj
{
private:
    enum {LF = 0, RF, RR, LR};
public:
    CTank();
    virtual ~CTank();
public:
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    void Input_Key();
    void Update_WorldMatrix();
private:

    D3DXVECTOR3 m_tTankBody[4];
    float       m_fSize;
    float       m_fGunLength;
    float       m_fAngle;
};

