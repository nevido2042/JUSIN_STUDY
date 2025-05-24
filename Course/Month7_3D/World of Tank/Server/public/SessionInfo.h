#pragma once
#include "PacketType.h"

typedef struct tagSessionInfo
{
	_int			iID = { 0 };
	_bool			isJoinMatch = { false };
	_float3			vPosition = {};
	TANK			eTank = { TANK::END };

	_float4x4		Matrix_Body = {};
	_float4x4		Matrix_Turret = {};
	_float4x4		Matrix_Gun = {};

	//�̷� �͵��� ���ο� ������ ���� �� �������
	//ĳ������ ����
	//ĳ������ �̵� ����
}SESSION_INFO;
