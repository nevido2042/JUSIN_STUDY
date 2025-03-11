#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr final : public CBase
{
private:
    CKeyMgr();
    virtual ~CKeyMgr() = default;

public:
    void Update();   // �� �����Ӹ��� Ű ���� ������Ʈ
    bool Key_Pressing(int _Key);
    bool Key_Up(int _Key);   // ������ �ִٰ� ���� ��
    bool Key_Down(int _Key); // ������ ��

private:
    bool m_bKeyState[VK_MAX];      // ���� ������ Ű ����
    bool m_bPrevKeyState[VK_MAX];  // ���� ������ Ű ����

public:
    static CKeyMgr* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual void Free() override;
};

END
