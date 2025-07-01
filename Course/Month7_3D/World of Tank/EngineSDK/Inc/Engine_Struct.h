#pragma once

namespace Engine
{
	typedef struct tagEngineDesc 
	{
		HINSTANCE		hInst;
		HWND			hWnd;
		_bool			isWindowed;
		_uint			iWinSizeX;
		_uint			iWinSizeY;
		_uint			iNumLevels;
		_uint			iNumColliderGroup;
	}ENGINE_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE				eType;
		_float4			vDirection;
		_float4			vPosition;
		float				fRange;

		_float4			vDiffuse;
		_float			fAmbient;
		_float4			vSpecular;
	}LIGHT_DESC;

	typedef struct tagMeshDesc
	{
		vector<_float3> Positions;
		vector<_float3> Normals;
		vector<_float3> Tangents;
		vector<_float2> TexCoords;
		vector<_uint> Indices;
		_uint MaterialIndex = 0;
	}MESH_DESC;


	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		_float3		vPosition;
		_float2		vTexcoord;

		static const _uint							iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexcoord
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexcoord;

		static const _uint							iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexMesh
	{
		_float3		vPosition;
		_float3		vNormal;
		_float3		vTangent;
		_float2		vTexcoord;

		static const _uint							iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertexRectParticleInstance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;
		XMFLOAT2		vLifeTime;

		static const unsigned int					iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXRECT_PARTICLE_INSTANCE;

	typedef struct ENGINE_DLL tagVertexPosition
	{
		XMFLOAT3		vPosition;

		static const unsigned int					iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertexPointParticleInstance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;
		XMFLOAT2		vLifeTime;

		static const unsigned int					iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOS_PARTICLE_INSTANCE;

	typedef struct ENGINE_DLL tagVertexPointInstance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;

		static const unsigned int					iNumElements = { 5 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOINT_INSTANCE;

	//struct QuadTreeNode
	//{
	//	_float3 vMin, vMax; // AABB
	//	vector<_uint> Indices; // »ï°¢Çü ÀÎµ¦½º ¸ñ·Ï
	//	QuadTreeNode* pChildren[4] = { nullptr, nullptr, nullptr, nullptr };
	//	_bool IsLeaf = false;

	//	~QuadTreeNode()
	//	{
	//		for (int i = 0; i < 4; ++i)
	//			if (pChildren[i]) delete pChildren[i];
	//	}
	//};
}