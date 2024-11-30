#pragma once

#include "Item.h"

class AddBullet : public Item
{
public:
	AddBullet();
	~AddBullet();

private:

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC _hdc) override;
	void Release() override;
	Obj* UseItem() override;
};

