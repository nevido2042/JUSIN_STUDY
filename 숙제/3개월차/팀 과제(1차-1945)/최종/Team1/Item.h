#pragma once
#include "Obj.h"
#include "Player.h"
class Item :public Obj
{
public:
	Item();
	virtual ~Item();
public: //Item ���� �Ӽ�
	virtual void Initialize()	override; 
	virtual int Update()		override;
	virtual void Late_Update()	override;
	virtual void Release() override;
public:
	virtual void Render(HDC _hdc) PURE;
	virtual void UseITem() PURE;
};

