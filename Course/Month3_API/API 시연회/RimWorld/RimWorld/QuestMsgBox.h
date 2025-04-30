#pragma once
#include "MsgBox.h"
class CQuestMsgBox :
    public CMsgBox
{
public:
    CQuestMsgBox();
    virtual ~CQuestMsgBox();
public:
    void Initialize() override;
    void Render(HDC hDC) override;
};

