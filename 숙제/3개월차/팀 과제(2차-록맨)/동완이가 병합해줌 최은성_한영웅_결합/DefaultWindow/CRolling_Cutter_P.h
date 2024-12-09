#pragma once
#include "CRolling_Cutter.h"
class CRolling_Cutter_P :
    public CRolling_Cutter
{
public :
    CRolling_Cutter_P();
    virtual ~CRolling_Cutter_P();

    void Initialize() override;
    int Update() override;
    void Late_Update() override;

private:
    float       m_fTime;
    POINT       m_tStartPoint;
};

