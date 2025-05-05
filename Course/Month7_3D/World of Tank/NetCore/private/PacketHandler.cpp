//#include "PacketHandler.h"
//#include "Packet.h"



#include "../public/PacketHandler.h"
#include "../public/Packet.h"

void CPacketHandler::mp_SC_CreateMyCharacter(CPacket* pPacket, int iID, _float3& _pStartPos)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//�ϴ� ���� ���̷�
    tHeader.byType = ENUM_CLASS(PacketType::SC_CREATE_MY_CHARACTER);

    //wprintf_s(L"mp_headerSize:%d\n", sizeof(header));

    pPacket->Put_Data((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << iID;
    *pPacket << _pStartPos;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_CreateMyCharacter(CPacket* pPacket, int& iID, _float3& _pStartPos)
{
    *pPacket >> iID;
    *pPacket >> _pStartPos;

    pPacket->Clear();
}

void CPacketHandler::mp_SC_CreateOtherCharacter(CPacket* pPacket, int iID, _float3& Pos)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    //tHeader.BYTEbySize = 0;//�ϴ� ���� ���̷�
    tHeader.byType = ENUM_CLASS(PacketType::SC_CREATE_OTHER_CHARACTER);

    pPacket->Put_Data((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << iID;
    *pPacket << Pos;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_CreateOtherCharacter(CPacket* pPacket, int& iID, _float3& Pos)
{
    *pPacket >> iID;
    *pPacket >> Pos;

    pPacket->Clear();
}

void CPacketHandler::mp_SC_DeleteCharacter(CPacket* pPacket, int iID)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = ENUM_CLASS(PacketType::SC_DELETE_CHARACTER);

    pPacket->Put_Data((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << iID;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_DeleteCharacter(CPacket* pPacket, int& iID)
{
    *pPacket >> iID;

    pPacket->Clear();
}
