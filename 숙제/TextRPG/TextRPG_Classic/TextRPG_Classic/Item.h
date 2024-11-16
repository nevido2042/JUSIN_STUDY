#pragma once
#include "Obj.h"
class CItem abstract:
    public CObj
{
protected:
    int m_iPrice;

public:
    CItem();
    virtual ~CItem();
public:
    void Initialize() = 0;
    void Update() override;
    void Release() override;
    void Render() override;
public:
    int Get_Price()
    {
        return m_iPrice;
    }
public:
    virtual void Use() = 0;
};

