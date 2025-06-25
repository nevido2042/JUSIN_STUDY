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

	}POINT_INSTANCE_DESC;

private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pDesc);
	virtual HRESULT Initialize(void* pArg);

	virtual void Drop(_float fTimeDelta)override;
	virtual void Spread(_float fTimeDelta)override;

	virtual void	Change_NumInstance(_int iNumInstance) override;
	virtual void	Change_Range(_float3 vRange) override;
	virtual void	Change_Size(_float2 vSize) override;
	void			Change_Pivot(_float3 vPivot);
	void			Change_LifeTime(_float2 vLifeTime);
	void			Change_Speed(_float2 vSpeed);
	void			Change_isLoop(_bool bLoop);
	virtual void Replay() override;


protected:
	VTXPOS_PARTICLE_INSTANCE* m_pVertexInstances = { nullptr }; //���
	_float* m_pSpeeds = { nullptr };
	_float3						m_vPivot = {};
	_bool						m_isLoop = { false };

	//�ν��Ͻ� ���� ������ �� �ʱ�ȭ�ϱ����� ����
	POINT_INSTANCE_DESC			m_tPointInstanceDesc = {};

public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END