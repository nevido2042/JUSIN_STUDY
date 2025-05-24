#pragma once
#include <string>
#include <vector>

namespace DebugUtils
{
//#if defined(_DEBUG)
    void SetConsoleSize(int width, int height);
    void ShowConsole();
    void CloseConsole();
//#endif
}
