#include "CServer.h"

int main()
{
	CServer Server;

	if (Server.Initialize() == false)
	{
		return 0;
	}

	while (true)
	{
		if (Server.Update() == false)
		{
			break;
		}
	}
}