#pragma once
#include "Obj.h"
#include "Node.h"

class CRim :
    public CObj
{
public:
    CRim();
    virtual ~CRim();
public:
    void Move_To(POS _Pos);
   
private:
    void Navigate();
    void Calculate_MoveDir();
    //이미지 좌우반전
    BOOL Flip_Image(HDC hdcDest, HDC hdcSrc, int xDest, int yDest, int width, int height);

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
private:
    list<CNode*>    m_NodeList;
    bool            m_bNavigating;
    POS             m_tPrevPos;
};
