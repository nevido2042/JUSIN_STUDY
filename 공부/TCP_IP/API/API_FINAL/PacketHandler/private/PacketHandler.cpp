#include "PacketHandler.h"

void CPacketHandler::mp_SC_CreateMyCharacter(CPacket* clpPacket, int iID, int iX, int iY)
{
    tagPACKET_HEADER tHeader{};
    tHeader.BYTEbyCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.BYTEbyType = PACKET_SC_CREATE_MY_CHARACTER;

    //wprintf_s(L"mp_headerSize:%d\n", sizeof(header));

    clpPacket->PutData((char*)&tHeader, sizeof(tHeader));
    *clpPacket << iID;
    *clpPacket << iX;
    *clpPacket << iY;

    clpPacket->UpdateHeaderSize(clpPacket->GetDataSize() - sizeof(tHeader));
}

void CPacketHandler::net_CreateMyCharacter(CPacket* clpPacket, int& iID, int& iX, int& iY)
{
    *clpPacket >> iID;
    *clpPacket >> iX;
    *clpPacket >> iY;

    clpPacket->Clear();
}

void CPacketHandler::mp_SC_CreateOtherCharacter(CPacket* clpPacket, int iID, int iX, int iY)
{
    tagPACKET_HEADER tHeader{};
    tHeader.BYTEbyCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//일단 고정 길이로
    tHeader.BYTEbyType = PACKET_SC_CREATE_OTHER_CHARACTER;

    clpPacket->PutData((char*)&tHeader, sizeof(tHeader));
    *clpPacket << iID;
    *clpPacket << iX;
    *clpPacket << iY;

    clpPacket->UpdateHeaderSize(clpPacket->GetDataSize() - sizeof(tHeader));
}

void CPacketHandler::net_CreateOtherCharacter(CPacket* clpPacket, int& iID, int& iX, int& iY)
{
    *clpPacket >> iID;
    *clpPacket >> iX;
    *clpPacket >> iY;

    clpPacket->Clear();
}
