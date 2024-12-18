#pragma once
#include "Pawn.h"
#include "Node.h"

#include "BoltActionRifle.h"

class CRim :
    public CPawn
{
public:
    enum STATE { DRAFTED, UNDRAFTED, DECONSTRUCTING, CONSTRUCTING, END };
public:
    CRim();
    virtual ~CRim();
public:
    STATE   Get_State() { return m_eState; }
    void    Change_State(STATE _eState);
    void    Set_TaskCheck() { m_bTaskCheck = true; }
public:
    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
private:
    //소집된 상태일 때
    void Drafed();
    //소집 안된 상태 일 때
    void Undrafed();
    //해체 중일때
    void Deconstruct();
    //건설 중일때
    void Construct();


private:
    void Check_DeconstructWork();
    void Check_ConstructWork();
private:
    STATE   m_eState;

    bool    m_bTaskCheck;
};
