#include "pch.h"

#define STDCALL __stdcall
#define CDECL __cdecl

void STDCALL MyStdcallFunction(const char* msg) {
    printf("MyStdcallFunction: %s\n", msg);
}

int main() {
    // 호출 규약을 매크로로 관리해 잘못된 사용 방지
    void (CDECL * myFunctionPointer)(const char*) = (void (CDECL*)(const char*))MyStdcallFunction;
    myFunctionPointer("Using macro for consistency");
    return 0;
}
