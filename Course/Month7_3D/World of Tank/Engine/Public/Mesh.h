#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(vector<_float3>& positions, vector<_float3>& normals, vector<_float3>& tangents, vector<_float2>& uvs, vector<_uint>& indices, _uint materialIndex);
	virtual HRESULT Initialize(void* pArg);


private:
	_uint			m_iMaterialIndex = {};

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, vector<_float3>& positions, vector<_float3>& normals, vector<_float3>& tangents, vector<_float2>& uvs, vector<_uint>& indices, _uint materialIndex = 0);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END