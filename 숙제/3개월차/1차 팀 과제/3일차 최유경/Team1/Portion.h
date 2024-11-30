#pragma once

#include "Obj.h"
#include "Item.h"

class Portion : public Item
{
public:
	Portion();
	virtual~Portion();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hdc) override;
	void Release() override;
	Obj* UseItem() override;

};

