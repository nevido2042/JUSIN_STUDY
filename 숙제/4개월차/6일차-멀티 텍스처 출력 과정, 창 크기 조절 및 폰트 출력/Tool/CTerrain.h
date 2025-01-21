#pragma once

#include "Include.h"

class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Release();

private:
	vector<TILE*>		m_vecTile;
};

