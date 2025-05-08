//#include "PacketHandler.h"
//#include "Packet.h"



#include "PacketHandler.h"
#include "Packet.h"

void CPacketHandler::mp_CS_Ping(CPacket* pPacket, void* pArg)
{
    pPacket->Clear();

    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = ENUM_CLASS(PacketType::CS_PING);

    pPacket->Put_Data((_byte*)&tHeader, sizeof(tHeader));

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::mp_SC_Ping(CPacket* pPacket)
{
    pPacket->Clear();

    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = ENUM_CLASS(PacketType::SC_PING);

    pPacket->Put_Data((_byte*)&tHeader, sizeof(tHeader));

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::mp_CS_Position(CPacket* pPacket, void* pArg)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = ENUM_CLASS(PacketType::CS_POSITION);

    POSITION_DESC* Position_Desc = static_cast<POSITION_DESC*>(pArg);

    pPacket->Put_Data((_byte*)&tHeader, sizeof(tHeader));
    *pPacket << Position_Desc->vPos;

    pPacket->Update_HeaderSize(pPacket->Get_DataSize() - sizeof(tHeader));
}

void CPacketHandler::net_Position(CPacket* pPacket, _float3& vPos)
{
    *pPacket >> vPos;

    pPacket->Clear();
}

void CPacketHandler::mp_SC_CreateMyCharacter(CPacket* pPacket, int iID, _float3& _pStartPos)
{
    tagPACKET_HEADER tHeader{};
    tHeader.byCode = PACKET_CODE;
    tHeader.byType = ENUM_CLASS(PacketType::SC_CREATE_MY_CHARACTER);

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
