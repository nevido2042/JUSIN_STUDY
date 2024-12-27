#pragma once
#include "Pawn.h"
#include "Node.h"

#include "BoltActionRifle.h"

class CRim :
    public CPawn
{

public:
    CRim();
    virtual ~CRim();
public:
    void    PutDown_Item();
public:
    void    Set_TaskCheck() { m_bTaskCheck = true; }
public:
    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
protected:
    void Handle_Wandering() override;
    void Handle_Drafted() override;
    void Handle_Undrafted() override;
    void Handle_Deconstructing() override;
    void Handle_Constructing() override;
    void Handle_Transporting() override;
private:
    //작업 목표물에 도달했는지 체크
    void Check_CloseTask();
    //해체
    void Deconstruct();
    //건설
    void Construct();

private:
    void Check_DeconstructWork();
    void Check_ConstructWork();
    //void Check_TransportingWork();

    void    Find_Enemy();
    CObj*   Find_Item(const TCHAR* _pImgKey);
    void    PickUp_Item(CObj* _pObj);

    bool    m_bTaskCheck;
    CObj*   m_pTransportingItem;
};
