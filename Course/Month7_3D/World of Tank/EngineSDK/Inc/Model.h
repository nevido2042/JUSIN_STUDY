#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
protected:
	CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _char* pModelFilePath);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex = 0);

public:
	virtual HRESULT Initialize_Prototype(MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);



private:
	string m_ModelFilePath = { nullptr };
private:
	_uint					m_iNumMeshes = {};
	vector<class CMesh*>	m_Meshes;

	MODEL					m_eType = {};
	_float4x4				m_PreTransformMatrix = {};
	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

private:
	HRESULT Load_Binary_Model(MODEL eType, const _char* pModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END