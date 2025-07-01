#include "Engine_Shader_Defines.hlsli"

matrix  g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_DiffuseTexture[4];
Texture2D g_MaskTexture;

float4 g_vLightDir;
float4 g_vLightDiffuse;
float4 g_vLightAmbient;
//float4 g_vLightSpecular;

float4 g_vCamPosition;

float4 g_vMtrlAmibient = float4(0.5f, 0.5f, 0.5f, 1.f);
float4 g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

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
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    //vector vSourMtrlDiffuse = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 1.f);
    //vector vDestMtrlDiffuse = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexcoord * 50.f);
    
    //vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord);
    float r = vMask.r;
    float g = vMask.g;
    float b = vMask.b;
    //float rest = max(0.f, 1.f - (r + g + b));
    
    // RGB 평균값 사용 (혹은 밝기 기준)
    float brightness = (r + g + b) / 3.f;

    vector tex0 = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 100.f);
    vector tex1 = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexcoord * 100.f);
    vector tex2 = g_DiffuseTexture[2].Sample(DefaultSampler, In.vTexcoord * 100.f);
    vector tex3 = g_DiffuseTexture[3].Sample(DefaultSampler, In.vTexcoord * 100.f);

    // 기준치 (예: 밝기 0.95 이상이면 흰색으로 간주)
    bool isWhite = brightness > 0.50f;
    
    // 일반 블렌딩
    //float rest = max(0.f, 1.f - (r + g + b));
    vector vBlend = tex0 * r + tex1 * g + tex2 * b;/* + tex3 * rest;*/

    // 흰색이면 tex3 사용, 아니면 블렌드
    vector vMtrlDiffuse = isWhite ? tex3 : vBlend;
    
   // vector vMtrlDiffuse = tex0 * r + tex1 * g + tex2 * b + tex3 * rest;
    
    
    //vector vMtrlDiffuse = vDestMtrlDiffuse * vMask + vSourMtrlDiffuse * (1.f - vMask);
    //g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord /** 100.f*/);
    
    float4 vShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f) +
        (g_vLightAmbient * g_vMtrlAmibient);
    
    float4 vLook = In.vWorldPos - g_vCamPosition;    
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    //float4 vSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 1000.f);
    
    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * vShade; + (/*g_vLightSpecular * */g_vMtrlSpecular); // * vSpecular;

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
