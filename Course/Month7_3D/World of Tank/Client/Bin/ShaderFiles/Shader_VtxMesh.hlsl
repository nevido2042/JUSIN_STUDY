#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_DiffuseTexture;

float4 g_vLightDir;
float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vLightSpecular;

float4 g_vCamPosition;

float4 g_vMtrlAmibient = float4(0.5f, 0.5f, 0.5f, 1.f);
float4 g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

float2  g_UVOffset = float2(0.f, 0.f);

float4 g_vBaseColor = float4(1.f, 1.f, 1.f, 1.f);


struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{      
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

// === ���� ���̴� ===
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : ��� ����� ���ϱ⸦ �����Ѵ�. /w������ �������� �ʴ´�. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    float2 uv = In.vTexcoord;

    uv += g_UVOffset;

    Out.vTexcoord = uv;
    
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    
    return Out;
}

// === �ȼ� ���̴� ===
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
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float4 vShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f) +
        (g_vLightAmbient * g_vMtrlAmibient);
    
    float4 vLook = In.vWorldPos - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float4 vSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 50.f);
        
    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * vShade + (g_vLightSpecular * g_vMtrlSpecular) * vSpecular;
    
    return Out;    
}

PS_OUT PS_BASECOLOR(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_vBaseColor * g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float4 vShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f) +
        (g_vLightAmbient * g_vMtrlAmibient);
    
    float4 vLook = In.vWorldPos - g_vCamPosition;
    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float4 vSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), 50.f);
        
    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * vShade + (g_vLightSpecular * g_vMtrlSpecular) * vSpecular;
    
    return Out;
}

PS_OUT PS_SKY(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

        
    Out.vColor = vMtrlDiffuse;
    
    return Out;
}

// === ��ũ�� ===
technique11 DefaultTechnique
{
    //0
    pass Default 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();        
        PixelShader = compile ps_5_0 PS_MAIN();      
    }
    //1
    pass Sky 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_SKY();
    }
    //2
    pass Tank
    {
        SetRasterizerState(RS_Cull_Front);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_BASECOLOR();
    }

    //3
    pass TigerGunSkin
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_BASECOLOR();
    }

   
}
