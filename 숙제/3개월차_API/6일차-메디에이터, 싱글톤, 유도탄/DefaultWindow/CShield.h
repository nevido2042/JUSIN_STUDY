#pragma once
#include "CObj.h"

class CShield : public CObj
{
public:
	CShield();
	virtual ~CShield();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	// CObj을(를) 통해 상속됨
	void Start() override;
};

