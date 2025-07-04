#include "AssimpBinaryConverter.h"

int main()
{
    _int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    //wstring wPath = L"../bin/Untitled.fbx";
    wstring wPath = argv[1];

    CAssimpBinaryConverter Converter;
    if (FAILED(Converter.Initialize(wPath)))
    {
        cout << "Initialize failed" << endl;
    }


    return 0;
}
