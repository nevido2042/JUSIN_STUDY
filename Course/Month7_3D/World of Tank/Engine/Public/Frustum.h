#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CFrustum final : public CBase
{
private:
    CFrustum();
    virtual ~CFrustum() = default;

public:
    HRESULT	Initialize();
    HRESULT Update();
public:
    _bool   Is_In_Frustum(_fvector vPos);
    _bool   Is_In_Frustum(_fvector vPos, _float fRadius);

private:
    class CGameInstance*    m_pGameInstance = { nullptr };

    _vector     m_FrustumPlane[6] = {};

public:
    static CFrustum* Create();
    virtual void Free() override;
};

NS_END

