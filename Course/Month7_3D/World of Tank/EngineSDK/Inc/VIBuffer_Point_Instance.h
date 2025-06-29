#pragma once

#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagPointInstance : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_float3		vPivot;
		_float2		vLifeTime;
		_float2		vSpeed;
		_bool		isLoop;
		_float		fAlpha;

	}POINT_INSTANCE_DESC;

private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pDesc);
	virtual HRESULT Initialize(void* pArg);

public:
	virtual void Drop(_float fTimeDelta) override;
	virtual void Spread(_float fTimeDelta) override;

public:
	virtual void	Change_NumInstance(_uint iNumInstance) override;
	virtual void	Change_Range(_float3 vRange) override;
	virtual void	Change_Size(_float2 vSize) override;
	void			Change_Pivot(_float3 vPivot);
	void			Change_LifeTime(_float2 vLifeTime);
	void			Change_Speed(_float2 vSpeed);
	void			Change_isLoop(_bool bLoop);
	void			Change_Alpha(_float fAlpha);
	void			Change_Translation(const vector<_float3>& Translations);
	void			Change_Matrix(const vector<_float4x4>& Matrixs);

	void			Change_Desc(const POINT_INSTANCE_DESC& Desc);
	virtual	void	Replay() override;

public:
	const POINT_INSTANCE_DESC& Get_Desc() const {
		return m_tPointInstanceDesc;
	}

protected:
	VTXPOS_PARTICLE_INSTANCE* m_pVertexInstances = { nullptr }; //요놈
	_float* m_pSpeeds = { nullptr };
	//_float3						m_vPivot = {};
	//_bool						m_isLoop = { false };

	//인스턴스 갯수 변경할 때 초기화하기위해 저장
	POINT_INSTANCE_DESC			m_tPointInstanceDesc = {};

public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END