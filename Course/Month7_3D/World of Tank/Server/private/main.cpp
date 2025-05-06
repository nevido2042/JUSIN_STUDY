#include "Engine_Defines.h"
#include "Server.h"

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    CServer Server;
    if (!Server.Initialize())
        return 0;

    while (Server.Update());

}
