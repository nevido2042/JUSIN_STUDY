#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CQuadTreeNode : public CBase
{
private:
	CQuadTreeNode();
	virtual ~CQuadTreeNode() = default;

public:
	HRESULT Initialize();

public:
	_float3&			Get_Min() { return m_vMin; }
	_float3&			Get_Max() { return m_vMax; }
	_bool				Is_Leaf() { return m_IsLeaf; }
	void				Set_Leaf() { m_IsLeaf = true; }
	vector<_uint>&		Get_Indices() { return m_Indices; }
	CQuadTreeNode**		Get_Children() { return m_pChildren; }
private:
	_float3			m_vMin = {};
	_float3			m_vMax = {};// AABB
	vector<_uint>	m_Indices = {}; // »ï°¢Çü ÀÎµ¦½º ¸ñ·Ï
	CQuadTreeNode*	m_pChildren[4] = { nullptr, nullptr, nullptr, nullptr };
	_bool			m_IsLeaf = { false };


public:
	static CQuadTreeNode* Create();
	virtual void Free() override;
};

NS_END