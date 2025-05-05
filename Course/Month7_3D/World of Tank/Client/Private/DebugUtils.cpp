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

    //void ClearConsole()
    //{
    //    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //    if (hConsole == INVALID_HANDLE_VALUE) return;

    //    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //    DWORD dwWritten;
    //    DWORD dwCells;

    //    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    //    dwCells = csbi.dwSize.X * csbi.dwSize.Y;

    //    FillConsoleOutputCharacter(hConsole, ' ', dwCells, { 0, 0 }, &dwWritten);
    //    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwCells, { 0, 0 }, &dwWritten);
    //    SetConsoleCursorPosition(hConsole, { 0, 0 });
    //}

    void CloseConsole()
    {
        FreeConsole();
    }
}
#endif
