#pragma once
#include "Obj.h"
class Monster :public Obj
{
public: 
	Monster();
	virtual ~Monster();
public:
	void        Set_ItemList(list<Obj*>* _pItemList) { m_pItemList = _pItemList; }
	void        Set_SatelliteList(list<Obj*>* _pSatelliteList) { m_pSatelliteList = _pSatelliteList; }
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
	list<Obj*>* m_pItemList;
	list<Obj*>* m_pSatelliteList;
};

