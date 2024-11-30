#pragma once
#include "Obj.h"

class Item : public Obj
{
public:
	Item() {};
	virtual~Item() {};

public:
	virtual void Initialize()PURE;
	virtual int Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hdc)PURE;
	virtual void Release()PURE;
	virtual Obj* UseItem()PURE;

protected:
	


public:
	
};
