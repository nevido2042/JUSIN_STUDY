#pragma once
#include "Pawn.h"
#include "Node.h"

#include "BoltActionRifle.h"

class CRim :
    public CPawn
{
public:
    enum STATE { DRAFTED, UNDRAFTED, WORKING, END };
public:
    CRim();
    virtual ~CRim();
public:
    STATE   Get_State() { return m_eState; }
    void    Change_State(STATE _eState) { m_eState = _eState;  m_pTarget = nullptr; }
public:
    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
private:
    void Deconstruct();
    //������ ������ ��
    void Drafed();
    //���� �ȵ� ���� �� ��
    void Undrafed();
    //���ϴ� �� �϶�
    void Work();
private:
    STATE   m_eState;
    bool    m_bFindingTask;
};
