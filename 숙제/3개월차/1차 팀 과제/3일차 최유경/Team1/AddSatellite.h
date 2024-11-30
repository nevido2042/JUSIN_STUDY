#pragma once

#include "Item.h"

class AddSatellite : public Item
{
public:
	AddSatellite();
	~AddSatellite();

private:


	// Item을(를) 통해 상속됨
	void Initialize() override;

	int Update() override;

	void Late_Update() override;

	void Render(HDC _hdc) override;

	void Release() override;

	Obj* UseItem() override;

};
