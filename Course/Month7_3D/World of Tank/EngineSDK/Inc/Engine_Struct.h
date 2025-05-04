#pragma once

namespace Engine
{
	typedef struct tagEngineDesc 
	{
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		unsigned int	iNumLevels;		
	}ENGINE_DESC;

	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;
	}VTXPOSTEX;

}