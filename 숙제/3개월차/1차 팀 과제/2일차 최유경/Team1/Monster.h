#pragma once
#include "Obj.h"
class Monster :public Obj
{
public: 
	Monster();
	~Monster();
public:
	void        Set_ItemList(list<Obj*>* _pItemList) { m_ItemList = _pItemList; }
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hdc) override;
	virtual void Release() override;
private:
	void Drop_Item();
private:
	int m_dir;
	list<Obj*>* m_ItemList;
};

