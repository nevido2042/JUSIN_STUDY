#include "PacketHandler.h"

void CPacketHandler::SC_CreateMyCharacter(CPacket* pPacket, int iID, int iX, int iY)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.byType = PACKET_SC_CREATE_MY_CHARACTER;

    //wprintf_s(L"mp_headerSize:%d\n", sizeof(header));

    pPacket->PutData((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << iID;
    *pPacket << iX;
    *pPacket << iY;

    pPacket->UpdateHeaderSize(pPacket->GetDataSize() - sizeof(tHeader));
}

void CPacketHandler::Recive_CreateMyCharacter(CPacket* pPacket, int& iID, int& iX, int& iY)
{
    *pPacket >> iID;
    *pPacket >> iX;
    *pPacket >> iY;

    pPacket->Clear();
}

void CPacketHandler::SC_CreateOtherCharacter(CPacket* pPacket, int iID, int iX, int iY)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.byType = PACKET_SC_CREATE_OTHER_CHARACTER;

    pPacket->PutData((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << iID;
    *pPacket << iX;
    *pPacket << iY;

    pPacket->UpdateHeaderSize(pPacket->GetDataSize() - sizeof(tHeader));
}

void CPacketHandler::Recive_CreateOtherCharacter(CPacket* pPacket, int& iID, int& iX, int& iY)
{
    *pPacket >> iID;
    *pPacket >> iX;
    *pPacket >> iY;

    pPacket->Clear();
}

void CPacketHandler::SC_DeleteCharacter(CPacket* pPacket, int iID)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = PACKET_SC_DELETE_CHARACTER;

    pPacket->PutData((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << iID;

    pPacket->UpdateHeaderSize(pPacket->GetDataSize() - sizeof(tHeader));
}

void CPacketHandler::Recive_DeleteCharacter(CPacket* pPacket, int& iID)
{
    *pPacket >> iID;

    pPacket->Clear();
}

void CPacketHandler::CS_DeleteMyCharacter(CPacket* pPacket)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = PACKET_CS_DELETE_MY_CHARACTER;

    pPacket->PutData((_byte*)&tHeader, sizeof(tHeader));

    pPacket->UpdateHeaderSize(pPacket->GetDataSize() - sizeof(tHeader));
}
