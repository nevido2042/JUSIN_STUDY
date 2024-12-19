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
private:
    //�۾� ��ǥ���� �����ߴ��� üũ
    void Check_CloseTask();
    //��ü
    void Deconstruct();
    //�Ǽ�
    void Construct();

private:
    void Check_DeconstructWork();
    void Check_ConstructWork();
    void Find_Enemy();

    bool    m_bTaskCheck;
};
