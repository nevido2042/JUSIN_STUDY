#pragma once

typedef struct tagSessionInfo
{
	_uint			iID = { 0 };
	_bool			isJoinMatch = { false };
	_float3			vPosition = {};

	_float4x4		Matrix_Body = {};
	_float4x4		Matrix_Turret = {};
	_float4x4		Matrix_Gun = {};

	//�̷� �͵��� ���ο� ������ ���� �� �������
	//ĳ������ ����
	//ĳ������ �̵� ����
}SESSION_INFO;
