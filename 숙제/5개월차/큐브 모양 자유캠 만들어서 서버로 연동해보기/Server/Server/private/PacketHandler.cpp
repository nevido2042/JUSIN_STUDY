#include "PacketHandler.h"
#include "Packet.h"

void CPacketHandler::mp_CS_Move_Start(CPacket* pPacket, _float3& _pStartPos)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.byType = PACKET_CS_MOVE_START;

    //wprintf_s(L"mp_headerSize:%d\n", sizeof(header));

    pPacket->Put_Data((_BYTE*)&tHeader, sizeof(tHeader));
    *pPacket << _pStartPos;
    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_Move_Start(CPacket* pPacket, _float3& _pStartPos)
{
    *pPacket >> _pStartPos;

    pPacket->Clear();
}

void CPacketHandler::mp_CS_Move_Stop(CPacket* pPacket, _float3& _pStartPos)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.byType = PACKET_CS_MOVE_START;

    //wprintf_s(L"mp_headerSize:%d\n", sizeof(header));

    pPacket->Put_Data((_BYTE*)&tHeader, sizeof(tHeader));
    *pPacket << _pStartPos;
    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_Move_Stop(CPacket* pPacket, _float3& _pStartPos)
{
    *pPacket >> _pStartPos;

    pPacket->Clear();
}

void CPacketHandler::mp_SC_CreateMyCharacter(CPacket* pPacket, int iID, int iX, int iY)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.byType = PACKET_SC_CREATE_MY_CHARACTER;

    //wprintf_s(L"mp_headerSize:%d\n", sizeof(header));

    pPacket->Put_Data((_BYTE*)&tHeader, sizeof(tHeader));
    *pPacket << iID;
    *pPacket << iX;
    *pPacket << iY;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_CreateMyCharacter(CPacket* pPacket, int& iID, int& iX, int& iY)
{
    *pPacket >> iID;
    *pPacket >> iX;
    *pPacket >> iY;

    pPacket->Clear();
}

void CPacketHandler::mp_SC_CreateOtherCharacter(CPacket* pPacket, int iID, int iX, int iY)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.byType = PACKET_SC_CREATE_OTHER_CHARACTER;

    pPacket->Put_Data((_BYTE*)&tHeader, sizeof(tHeader));
    *pPacket << iID;
    *pPacket << iX;
    *pPacket << iY;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_CreateOtherCharacter(CPacket* pPacket, int& iID, int& iX, int& iY)
{
    *pPacket >> iID;
    *pPacket >> iX;
    *pPacket >> iY;

    pPacket->Clear();
}

void CPacketHandler::mp_SC_DeleteCharacter(CPacket* pPacket, int iID)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = PACKET_SC_DELETE_CHARACTER;

    pPacket->Put_Data((_BYTE*)&tHeader, sizeof(tHeader));
    *pPacket << iID;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_DeleteCharacter(CPacket* pPacket, int& iID)
{
    *pPacket >> iID;

    pPacket->Clear();
}

void CPacketHandler::mp_CS_DeleteMyCharacter(CPacket* pPacket)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = PACKET_CS_DELETE_MY_CHARACTER;

    pPacket->Put_Data((_BYTE*)&tHeader, sizeof(tHeader));

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}
