// DebugUtils.cpp
#if defined(_DEBUG)
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

namespace DebugUtils
{
    void ShowConsole()
    {
        AllocConsole();
        SetConsoleTitleW(L"Debug Console");

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);

        SetConsoleOutputCP(CP_UTF8);
        std::ios::sync_with_stdio();
    }

    void CloseConsole()
    {
        FreeConsole();
    }
}
#endif
