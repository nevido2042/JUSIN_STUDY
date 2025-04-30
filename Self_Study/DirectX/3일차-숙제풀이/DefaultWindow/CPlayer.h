#pragma once
#include "CObj.h"

class CPlayer :    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();
     
public:
    void Initialize()       override;
    void Update()           override;
    void Render(HDC hDC)    override;
    void Release()          override;

private:
    void        Key_Input();

private:
    D3DXVECTOR3     m_vPoint[4];
    D3DXVECTOR3     m_vOriginPoint[4];

    D3DXVECTOR3     m_vGunPoint;
    D3DXVECTOR3     m_vOriginGunPoint;

    float           m_fAngle;

};

