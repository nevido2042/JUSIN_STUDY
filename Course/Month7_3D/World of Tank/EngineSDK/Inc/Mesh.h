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
	virtual HRESULT Initialize_Prototype(MODEL eType, MESH_DESC& Desc, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);


private:
	_uint			m_iMaterialIndex = {};

private:
	HRESULT Ready_NonAnim_Mesh(const MESH_DESC& Desc, _fmatrix PreTransformMatrix);
	HRESULT Ready_Anim_Mesh(const MESH_DESC& Desc);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MESH_DESC& Desc, MODEL eType, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END