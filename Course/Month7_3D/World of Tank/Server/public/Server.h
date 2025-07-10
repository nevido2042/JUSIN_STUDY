#pragma once
#include "Packet.h"
#include "Session.h"
#include "PacketType.h"
#include <vector>
#include <locale>
#include <iostream>

using namespace std;

#define BUF_SIZE 2048
//#define PORT L"2042"

class CServer final
{
public:
	CServer();
	~CServer();
public:
	_bool Initialize();
	_bool Update();
	void Release();
private:
	_bool Network();
	void AcceptProc();
	void Read_Proc(CSession* pSession);
private:
	void Recieve_Message();
	void Send_Message();
private:
	//void Send_Unicast(CSession* pSession, const _byte* tMSG, const int iSize);
	//void Send_Broadcast(CSession* pSession, const _byte* tMSG, const int iSize);
private:
	void Decode_Packet(const PACKET_HEADER& _Header, CSession* _pSession);
	void Delete_Dead_Session();
	void Check_Session_State(SOCKET sock);
	void Set_Session_Dead(CSession* pSession);
private:
	void Load_Config_File(const wstring& filename);

public:
	HRESULT Send_Packet_Unicast(CSession* pSession, _uint iPacketType, void* pArg = nullptr);
	HRESULT Send_Packet_Broadcast(CSession* pSession, _uint iPacketType, void* pArg = nullptr);
	HRESULT Ready_Packet(_uint iPacketType, function<void(void*)> pFunction);
public:
	HRESULT	Clear_Packet();
	HRESULT	Input_Data(_byte* pByte, _int iSize);
	HRESULT	Output_Data(_byte* pByte, _int iSize);
	HRESULT	Update_Header();
private:
	HRESULT Define_Packets();
private:
	CSession*	Find_Session(_int iID);
	HRESULT		Flush_SendBuffer(CSession* pSession);

private:
	SOCKET						m_ServSock = { INVALID_SOCKET };
	_uint						m_iPort = {};
	_int						m_iID = {0};

	vector<CSession*>			m_vecSession;

	fd_set						m_tReadSet;
	CPacket						m_Packet;

	_bool						m_bIsGameRunning = { false };
	_uint	m_iMaxPlayer = { 2 };

	//chrono::steady_clock::time_point m_StartTime;
	//_uint m_iGameDurationSec = 180; // 3Ка
	//_uint Get_RemainingTime();

	unordered_map<_uint, function<void(void*)>> m_PacketTypes;

};

