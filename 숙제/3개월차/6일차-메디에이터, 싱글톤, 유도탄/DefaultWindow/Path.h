#pragma once
#include "CObj.h"
class CPath :
    public CObj
{
public:
    CPath();
    virtual ~CPath();
public:
    void Add_Point(POINT _tPoint)
    {
        m_PointList.push_back(_tPoint);
    }
public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    list<POINT> m_PointList;

};

