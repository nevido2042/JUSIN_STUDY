#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CMaterial final : public CBase
{
private:
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial() = default;

public:
	HRESULT Initialize();
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, aiTextureType eType, _uint iTextureIndex);
	HRESULT Add_SRV(const _char* pTexPath, aiTextureType eType);


private:
	ID3D11Device*								m_pDevice = { nullptr };
	ID3D11DeviceContext*						m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>			m_SRVs[AI_TEXTURE_TYPE_MAX] = {};


public:
	static CMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
	

};

NS_END