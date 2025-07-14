#include "Engine_Shader_Defines.hlsli"

matrix g_ViewMatrix, g_ProjMatrix, g_WorldMatrix;

matrix g_WorldMatrixInv;
matrix g_ProjMatrixInv;
matrix g_ViewMatrixInv;


Texture2D g_DepthTexture;
Texture2D g_Texture;

float2 g_ScreenSize;

struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
};

VS_OUT VS_DECAL(VS_IN In)
{
    VS_OUT Out;
    float4 worldPos = mul(float4(In.vPos, 1.0f), g_WorldMatrix);
    Out.vPosition = mul(worldPos, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    return Out;
}

struct PS_OUT
{
    float4 vDiffuse : SV_Target0;
    float4 vDecal : SV_Target4;
};

PS_OUT PS_DECAL(VS_OUT In)
{
    PS_OUT Out;

    float2 uv = In.vPosition.xy / In.vPosition.w;
    uv = uv * 0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, uv);
    float fViewZ = vDepthDesc.y * 1000.f;

    // NDC 좌표 복원
    float4 vNDCPos;
    vNDCPos.x = uv.x * 2.f - 1.f;
    vNDCPos.y = uv.y * -2.f + 1.f;
    vNDCPos.z = vDepthDesc.x; // 투영 공간 Z
    vNDCPos.w = 1.f;

    // View Z 스케일링
    vNDCPos *= fViewZ;

    // WorldPos 복원
    float4 vWorldPos = mul(vNDCPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    // 데칼 로컬 공간으로 변환
    float3 vLocalPos = mul(float4(vWorldPos.xyz, 1.f), g_WorldMatrixInv).xyz;

    // 데칼 범위 바깥이면 버리기
    if (any(abs(vLocalPos) > 0.5f))
        discard;

    // 텍스처 좌표 계산 (0 ~ 1)
    float2 decalUV = vLocalPos.xy + 0.5f;

    float4 decalColor = g_Texture.Sample(DefaultSampler, decalUV);
    Out.vDiffuse = decalColor;
    Out.vDecal = decalColor;
    
    return Out;
}

struct VS_OUT_DEBUG
{
    float4 vPosition : SV_POSITION;
};

VS_OUT_DEBUG VS_DEBUG(VS_IN In)
{
    VS_OUT_DEBUG Out;
    float4 worldPos = mul(float4(In.vPos, 1.f), g_WorldMatrix);
    Out.vPosition = mul(worldPos, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    return Out;
}

float4 PS_DEBUG(VS_OUT_DEBUG In) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f); // 빨간 선
}



technique11 DefaultTechnique
{
    pass Decal
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0, 0, 0, 0), 0xffffffff);

        VertexShader = compile vs_5_0 VS_DECAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DECAL();
    }

    pass DebugCube // 디버깅용 큐브
    {
        SetRasterizerState(RS_Wireframe); // 와이어프레임으로 보이게
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0, 0, 0, 0), 0xffffffff);

        VertexShader = compile vs_5_0 VS_DEBUG();
        PixelShader = compile ps_5_0 PS_DEBUG();
    }
}
