#pragma once

#include "Client_Defines.h"
#include "Tool.h"
#include "VIBuffer_Point_Instance.h"

NS_BEGIN(Engine)
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CParticleTool final : public CTool
{
private:
	CParticleTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticleTool(const CParticleTool& Prototype);
	virtual ~CParticleTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	wstring OpenFileDialog();
	HRESULT Load_Texture(const wstring& strPrototypeTag, const wstring& pTextureFilePath);
	HRESULT Change_Texture(const wstring& strPrototypeTag);
	HRESULT Save_Particle(const wstring& ParticleName);


private:
	HRESULT Load_Particles();
	vector<wstring> Get_ParticleFileNames();
	//HRESULT Make_Particles_List();


	void ParticleControl();
	void ParticleList();

private:
	_int m_iShaderPass = { 0 };
private:
	_int m_SelectedIndex = { -1 };
	vector<CVIBuffer_Point_Instance::POINT_INSTANCE_DESC> m_ParticleDescs = {};
private:
	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC m_BaseParticleDesc = {};
private:
	static _uint	m_strPrototypeTag_ID;
private:
	CVIBuffer_Point_Instance* m_VIBuffer = { nullptr };
	class CBaseParticle*	m_pBaseParticle = { nullptr };
	_char m_szParticleName[MAX_PATH] = { "" };

public:
	static CParticleTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END