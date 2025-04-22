#pragma once
#include "CBullet.h"
class CTornadoBullet :
    public CBullet
{
public:
    CTornadoBullet();
    virtual ~CTornadoBullet();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hdc) override;
public:
    void Set_FocalPoint(POINT _tPoint) { m_tFocalPoint = _tPoint; }

private:
    POINT m_tFocalPoint;
    float m_fDistance;
    float m_fOrbitalAngle;
};

