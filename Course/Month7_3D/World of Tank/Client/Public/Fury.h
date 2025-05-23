#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CFury final : public CLandObject
{
public:
#pragma message ("탱크 디스크립션으로 바꾸면 좋을듯 티거랑 공용으로 쓰게")
	typedef struct tagFuryDesc : public GAMEOBJECT_DESC
	{
		_uint	iID = { SESSION_MAX };
	}FURY_DESC;
private:
	CFury(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFury(const CFury& Prototype);
	virtual ~CFury() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


public:
	_uint	Get_ID() const { return m_iID; }
private:
	void Destroyed();
private:
	void Move(_float fTimeDelta);

private:
	_uint	m_iID = { SESSION_MAX };

private:
	_bool	m_bDestroyed = { false };
private:
	ID3D11RasterizerState*	m_pRasterState = { nullptr };
	ID3D11RasterizerState*	m_pOldRasterState = { nullptr };
private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };
	CModel*		m_pModelCom_Destroyed = { nullptr };

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	HRESULT Bind_ShaderResources();
private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CFury* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END