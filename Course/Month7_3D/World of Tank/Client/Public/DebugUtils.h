// DebugUtils.h
#pragma once
#include <string>
#include <vector>

namespace DebugUtils
{
#if defined(_DEBUG)
    void ShowConsole();
    void ClearConsole();
    void CloseConsole();

    void Log(const std::string& line); // 로그 추가
    void Flush();                      // 콘솔에 출력하고 버퍼 비움
#endif
}
