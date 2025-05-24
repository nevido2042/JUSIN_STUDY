
//#if defined(_DEBUG)
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

    void SetConsoleSize(int width, int height)
    {
        // 콘솔 핸들 가져오기
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // 화면 버퍼 크기 설정
        COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
        SetConsoleScreenBufferSize(hConsole, bufferSize);

        // 콘솔 창 크기 설정
        SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    }

    void CloseConsole()
    {
        FreeConsole();
    }
}
//#endif
