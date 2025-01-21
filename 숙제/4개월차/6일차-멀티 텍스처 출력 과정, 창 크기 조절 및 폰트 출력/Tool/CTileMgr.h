#pragma once
#include "Include.h"
#include "CDevice.h"
#include "CTile.h"

class CTileMgr
{
	DECLARE_SINGLETON(CTileMgr)
public:
	void Initialize();
	void Render(CDevice* _pDevice);
	void Release();
private:
	CTileMgr();
	~CTileMgr();
private:
	vector<vector<CObj*>>	m_vecTileMap;
};

