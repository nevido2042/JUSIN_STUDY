#pragma once
#include "Item.h"
class Satellite_Item :
    public Item
{
public:
    Satellite_Item();
    virtual ~Satellite_Item();
public:
    void        Set_SatelliteList(list<Obj*>* _pSatelliteList) { m_pSatelliteList = _pSatelliteList; }
public:
    virtual void Upgrade_Target() override;
    virtual void Render(HDC _hDC) override;
private:
    list<Obj*>* m_pSatelliteList;
};

