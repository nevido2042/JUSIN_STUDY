#pragma once
#include "PacketType.h"

typedef struct tagSessionInfo
{
	_int			iID = { 0 };
	_bool			isJoinMatch = { false };
	_float3			vPosition = {};
	TANK			eTank = { TANK::END };

	//�̷� �͵��� ���ο� ������ ���� �� �������
	//ĳ������ ����
	//ĳ������ �̵� ����
}SESSION_INFO;
