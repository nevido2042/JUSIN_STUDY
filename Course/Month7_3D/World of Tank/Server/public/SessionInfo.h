#pragma once
#include "PacketType.h"

typedef struct tagSessionInfo
{
	//��������
	_int			iID = { 0 };
	_bool			isJoinMatch = { false };
	TANK			eTank = { TANK::END };
	_float4			vBodyColor = { 1.f, 1.f, 1.f, 1.f };
	_float4			vTurretColor = { 1.f, 1.f, 1.f, 1.f };
	_float4			vGunColor = { 1.f, 1.f, 1.f, 1.f };

	//�Ⱦ�������
	_float3			vPosition = {};

	//�̷� �͵��� ���ο� ������ ���� �� �������
	//ĳ������ ����
	//ĳ������ �̵� ����
}SESSION_INFO;
