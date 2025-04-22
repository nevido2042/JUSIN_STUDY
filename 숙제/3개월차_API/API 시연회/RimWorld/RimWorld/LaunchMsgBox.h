#pragma once
#include "MsgBox.h"
class CLaunchMsgBox :
    public CMsgBox
{
public:
    CLaunchMsgBox();
    virtual ~CLaunchMsgBox();
public:
    void Initialize() override;
    void Render(HDC hDC) override;
private:
    int m_iRimCount;
};

