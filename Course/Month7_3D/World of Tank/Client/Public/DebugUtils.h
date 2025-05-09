// DebugUtils.h
#pragma once
#include <string>
#include <vector>

namespace DebugUtils
{
#if defined(_DEBUG)
    void ShowConsole();
    void CloseConsole();
#endif
}
