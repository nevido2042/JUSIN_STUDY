#pragma once
#include "Object.h"
class CGame :
    public CObject
{
public:
    CGame();
    virtual ~CGame();

    // CObject��(��) ���� ��ӵ�
    void Initialize() override;
    void Update() override;
    void Release() override;
public:
    void Print_InputAnyKey_StartToPlay();
    void Start_Game();
};

