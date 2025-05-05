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

    void Log(const std::string& line); // �α� �߰�
    void Flush();                      // �ֿܼ� ����ϰ� ���� ���
#endif
}
