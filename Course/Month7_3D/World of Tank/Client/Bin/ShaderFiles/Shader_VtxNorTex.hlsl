#include "Engine_Shader_Defines.hlsli"

matrix  g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_DiffuseTexture[4];
Texture2D g_MaskTexture;

//float g_fFar; 일단 상수로 박아두고 사용

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    matrix matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matrix matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord);
    float r = vMask.r;
    float g = vMask.g;
    float b = vMask.b;

    // RGB 평균값 사용 (혹은 밝기 기준)
    float brightness = (r + g + b) / 3.f;

    vector tex0 = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 100.f);
    vector tex1 = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexcoord * 100.f);
    vector tex2 = g_DiffuseTexture[2].Sample(DefaultSampler, In.vTexcoord * 100.f);
    vector tex3 = g_DiffuseTexture[3].Sample(DefaultSampler, In.vTexcoord * 100.f);

    // 기준치 (밝기 일정 이상이면 흰색으로 간주)
    bool isWhite = brightness > 0.50f;
    
    // 일반 블렌딩
    vector vBlend = tex0 * r + tex1 * g + tex2 * b;

    // 흰색이면 tex3 사용, 아니면 블렌드
    vector vMtrlDiffuse = isWhite ? tex3 : vBlend;
    
    Out.vDiffuse = vector(vMtrlDiffuse.rgb, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
