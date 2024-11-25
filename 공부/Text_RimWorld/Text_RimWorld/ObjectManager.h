#pragma once
#include "Object.h"
#include <list>

class CObjectManager :
    public CObject
{
public:
    virtual ~CObjectManager();
public:
    // CObject��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Release() override;
public:
    void Add_Object(CObject* _pObject);
private:
    list<CObject*> m_ObjectList;
};

