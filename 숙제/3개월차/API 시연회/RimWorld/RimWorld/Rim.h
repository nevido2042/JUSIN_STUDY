#pragma once
#include "Pawn.h"
#include "Node.h"

#include "BoltActionRifle.h"

class CRim :
    public CPawn
{
public:
    enum Dir { NORTH, EAST, SOUTH, WEST };
public:
    CRim();
    virtual ~CRim();
public:
    void    PutDown_Item();
public:
    wstring* Get_ImgKeyArr_Body() { return &m_ImgKeyArr_Body[SOUTH]; }
    wstring* Get_ImgKeyArr_Face() { return &m_ImgKeyArr_Face[SOUTH]; }
    wstring* Get_ImgKeyArr_Hair() { return &m_ImgKeyArr_Hair[SOUTH]; }

    void    Set_TaskCheck() { m_bTaskCheck = true; }
public:
    void Initialize() override;
    int  Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
protected:
    void Handle_Wandering() override;
    void Handle_Drafted() override;
    void Handle_Undrafted() override;
    void Handle_Deconstructing() override;
    void Handle_Constructing() override;
    void Handle_Transporting() override;
    void Handle_Boarding() override;
    void Handle_Logging() override;
private:
    //작업 목표물에 도달했는지 체크
    void Check_CloseTask();
    //해체
    void Deconstruct();
    //건설
    void Construct();
    //벌목
    void Log();
private:
    void Check_DeconstructWork();
    void Check_ConstructWork();
    void Check_LoggingWork();
    //void Check_TransportingWork();

    void    Find_Enemy();
    CObj*   Find_Item(const TCHAR* _pImgKey);
    void    PickUp_Item(CObj* _pObj);

    bool    m_bTaskCheck;
    CObj*   m_pTransportingItem;

    //몸통, 얼굴, 머리
    wstring m_pImgKey_Body;
    wstring m_pImgKey_Face;
    wstring m_pImgKey_Hair;

    array<wstring, 4> m_ImgKeyArr_Body;
    array<wstring, 4> m_ImgKeyArr_Face;
    array<wstring, 4> m_ImgKeyArr_Hair;
   
};
