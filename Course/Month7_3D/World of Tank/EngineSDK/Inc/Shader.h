#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Float2(const _char* pConstantName, _float2* pFloat2);
	HRESULT Bind_Float(const _char* pConstantName, _float* pFloat);
	HRESULT Bind_Int(const _char* pConstantName, _int* pInt);

	//HRESULT Bind_TexOffset(const _char* pConstantName, const _float2* pTexOffset);
	//HRESULT Bind_TexScale(const _char* pConstantName, const _float2* pTexScale);

	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);

private:
	ID3DX11Effect*					m_pEffect = { nullptr };
	_uint							m_iNumPasses = { };
	vector<ID3D11InputLayout*>		m_InputLayouts;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END