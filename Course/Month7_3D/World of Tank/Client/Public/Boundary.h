#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END


NS_BEGIN(Client)

class CBoundary final : public CGameObject
{
private:
	CBoundary(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoundary(const CBoundary& Prototype);
	virtual ~CBoundary() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

//public:
//	_bool IsPointInBoundary(const _float3& vPoint, _float3& vOutClosestPos);

public:
	_bool IsPointInBoundary(const _float3& vPoint);
	_float3 SafeInsideBoundary(const _float3& vBoundaryPoint, const _float3& vCenter, _float fOffset = 0.01f);
	_float3 ClosestPointOnBoundary(const _float3& vPos);

//private:
//	_bool IsPointOnSegmentXZ(const _float3& p, const _float3& a, const _float3& b, _float fTolerance);

private:
	HRESULT Load_BoundaryPoints();

#ifdef _DEBUG
	//void Draw_Boundary();
#endif // _DEBUG

private:
	vector<_float3> m_BoundaryPoints;

	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

#ifdef _DEBUG
	//PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	//BasicEffect* m_pEffect = { nullptr };
	//ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif // _DEBUG

private:
	HRESULT Ready_Components();

public:
	static CBoundary* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END