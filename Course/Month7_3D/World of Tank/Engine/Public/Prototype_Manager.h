#pragma once

#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Rect_Instance.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Trail.h"
#include "VIBuffer_Rect.h"
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "SoundController.h"
#include "Model.h"

#include "Collider.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

/* 1. ������ü�� �����Ѵ�. */
/* 1_1. ������ü�� Ÿ��( CGameObject, CComponent )�� ũ�� ������ ���� �ʴ´�. */
/* 1_2. �����Ҷ� ������( + ��緹��) �� �����Ͽ� �����Ұž�. */

NS_BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT				Initialize(_uint iNumLevels);
	HRESULT				Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase*				Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	const PROTOTYPES&	Get_Prototypes(_uint iPrototypeLevelIndex) const;
	class CBase*		Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

	void Clear(_uint iLevelIndex);
private:
	_uint			m_iNumLevels = {};
	PROTOTYPES*		m_pPrototypes = { nullptr };

public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

NS_END