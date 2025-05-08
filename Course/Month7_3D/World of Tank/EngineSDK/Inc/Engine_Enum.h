#pragma once

namespace Engine
{
	enum class D3DTS { VIEW, PROJ, END };
	enum class RENDERGROUP{ RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };
	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };
	enum class STATE { RIGHT, UP, LOOK, POSITION, END };
	enum class DIMK { LBUTTON, RBUTTON, WHEEL, END };
	enum class DIMM { X, Y, WHEEL, END };
	enum class NETWORK_STATUS { CONNECTED, WARNING, DISCONNECTED };
}