#include "Server.h"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	CServer Server;

	if (Server.Initialize() == false)
	{
		return 0;
	}

	while (true)
	{
		/*if (GetKeyState(VK_ESCAPE) & 0x8000)
		{
			break;
		}*/

		if (Server.Update() == false)
		{
			break;
		}
	}
}