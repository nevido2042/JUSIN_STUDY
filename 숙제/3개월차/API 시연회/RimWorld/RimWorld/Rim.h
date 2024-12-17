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
    void    Change_State(STATE _eState);
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
    //���ϴ� �� �϶�
    void Work();

private:
    void Check_DeconstructWork();
    void Check_ConstructWork();
private:
    STATE   m_eState;

    //�۾� ã�� �ð� ����
    float   m_fTaskCheckInterval;

    //����ð� �ð� üũ
    float   m_fElapsedTimeCheck;
};
