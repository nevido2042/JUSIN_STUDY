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
    //������ ������ ��
    void Drafed();
    //���� �ȵ� ���� �� ��
    void Undrafed();
    //��ü ���϶�
    void Deconstructing();
    //�Ǽ� ���϶�
    void Constructing();

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
private:
    STATE   m_eState;

    bool    m_bTaskCheck;
};
