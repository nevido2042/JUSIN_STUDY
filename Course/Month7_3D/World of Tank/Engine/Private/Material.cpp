#include "Material.h"
#include "Shader.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext { pContext }    
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CMaterial::Initialize()
{
    return S_OK;
}
HRESULT CMaterial::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, aiTextureType eType, _uint iTextureIndex)
{
    if (iTextureIndex >= m_SRVs[eType].size())
        return E_FAIL;

    return pShader->Bind_SRV(pConstantName, m_SRVs[eType][iTextureIndex]);    
}
HRESULT CMaterial::Add_SRV(const _char* pTexPath, aiTextureType eType)
{
    if (nullptr == pTexPath)
        return E_FAIL;

    ID3D11ShaderResourceView* pSRV = nullptr;

    // 확장자 구분 (.dds면 DDS, 나머지는 WIC)
    _char szExt[_MAX_EXT] = {};
    _splitpath_s(pTexPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, _MAX_EXT);

#pragma message ("메테리얼 없는 메쉬가 있으면 터짐, 무조건 넣어줘야함")

    // 문자열을 wchar로 변환
    _tchar szWidePath[MAX_PATH] = {};
    MultiByteToWideChar(CP_ACP, 0, pTexPath, -1, szWidePath, MAX_PATH);

    HRESULT hr = E_FAIL;

    // 확장자가 .dds가 아니면 경로를 .dds로 변경하여 읽음
    if (0 != _stricmp(szExt, ".dds"))
    {
        // 확장자를 .dds로 바꿈
        std::string modifiedPath = pTexPath;
        size_t extPos = modifiedPath.find_last_of(".");
        if (extPos != std::string::npos)
        {
            modifiedPath.replace(extPos, 4, ".dds");
        }

        wstring temp = std::wstring(modifiedPath.begin(), modifiedPath.end());

        // .dds로 변경된 경로로 텍스처 로드
        hr = DirectX::CreateDDSTextureFromFile(m_pDevice, temp.c_str(), nullptr, &pSRV);
    }
    else
    {
        hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szWidePath, nullptr, &pSRV);
    }

    if (FAILED(hr))
        return E_FAIL;

    // Diffuse 슬롯 (1번)에 등록
    m_SRVs[eType].push_back(pSRV);

    return S_OK;
}
CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMaterial* pInstance = new CMaterial(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CMaterial");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CMaterial::Free()
{
    __super::Free();

    for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
    {
        for (auto& pSRV : m_SRVs[i])
            Safe_Release(pSRV);
        m_SRVs[i].clear();
    }

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
