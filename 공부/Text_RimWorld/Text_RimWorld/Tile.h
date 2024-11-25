#pragma once
#include "Object.h"
class CTile :
    public CObject
{
public:
    enum Type { NONE = 0, WALL, RIM };
public:
    CTile();
public:
    // CObject을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Release() override;
public:
    const Type& Get_Type() const
    {
        return m_Type;
    }
    void Set_Type(const Type _Type)
    {
        m_Type = _Type;
    }
private:
    Type m_Type;
};

