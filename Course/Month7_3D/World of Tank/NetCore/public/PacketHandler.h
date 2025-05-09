#pragma once

#include "Engine_Defines.h"
#include "Packet.h"

enum class PacketType
{
    SC_GIVE_ID,

    CS_PING,
    SC_PING,

    CS_POSITION,
    SC_POSITION,

	CS_CREATE_MY_CHARACTER,
    SC_CREATE_MY_CHARACTER,

	CS_CREATE_OTHER_CHARACTER,
    SC_CREATE_OTHER_CHARACTER,

	CS_DELETE_CHARACTER,
    SC_DELETE_CHARACTER,
};

#pragma message ("��Ŷ�ڵ鷯 �� ���ش�.  ��Ŷ���� �̵��ϸ� ���� ��?")
class CPacketHandler
{
public:
    typedef struct tagPacket_Desc
    {
        _uint iID{ /*SESSION_MAX*/ 10 }; //SESSION_MAX �� ����
    }PACKET_DESC;

    typedef struct tagPostion_Desc : PACKET_DESC
    {
        _float3 vPos{};
    }POSITION_DESC;

    //static void net_Position(CPacket* pPacket, _float3& vPos);

    //static void mp_SC_CreateMyCharacter(CPacket* pPacket, int iID, _float3& Pos);
    //static void net_CreateMyCharacter(CPacket* pPacket, int& iID, _float3& Pos);

    //static void mp_SC_CreateOtherCharacter(CPacket* pPacket, int iID, _float3& Pos);
    //static void net_CreateOtherCharacter(CPacket* pPacket, int& iID, _float3& Pos);

    //static void mp_SC_DeleteCharacter(CPacket* pPacket, int iID);
    //static void net_DeleteCharacter(CPacket* pPacket, int& iID);
};

