#pragma once
#include "Obj.h"
class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();
public:
    HRESULT Initialize(void) override;
    int Update(void) override;
    void Late_Update(void) override;
    void Render(void) override;
    void Release(void) override;
public:
    void Set_State(PLAYER_STATE _eState);
private:
    FRAME           m_tFrame;
    D3DXVECTOR3     m_vClickPos;
    float           m_fWalkSpeed;
    PLAYER_STATE    m_eState;
    const TCHAR*    m_pStateKey;

};
