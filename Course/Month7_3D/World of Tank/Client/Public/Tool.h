#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CTool abstract : public CBase
{
protected:
	CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

NS_END