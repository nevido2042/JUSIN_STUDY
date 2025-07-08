#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;

float2  g_UVOffset = float2(0.f, 0.f);

float4 g_vBaseColor = float4(1.f, 1.f, 1.f, 1.f);

float g_OutlineSize = 0.1f;

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
    float4 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

// === 정점 셰이더 ===
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix));
    Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    float2 uv = In.vTexcoord;
    uv += g_UVOffset;
    Out.vTexcoord = uv;
    
    return Out;
}

struct VS_OUT_OUTLINE
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

VS_OUT_OUTLINE VS_OUTLINE(VS_IN In)
{
    VS_OUT_OUTLINE Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    In.vPosition.xyz += In.vNormal * g_OutlineSize;
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};


VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}


// === 픽셀 셰이더 ===
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float4 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_IN_OUTLINE
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_OUTLINE
{
    //vector vDepth : SV_TARGET2;
    vector vOutline : SV_TARGET3;
    //vector vOutlineDepth : SV_TARGET4;
};


struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
    //vector vDepthOutline : SV_TARGET4;
};

struct PS_OUT_TANK
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
    vector vDepthOutline : SV_TARGET4;
};

struct PS_OUT_SKY
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    //if (vMtrlDiffuse.a < 0.3f)
    //    discard;
    
    //건물중에 투명한게 나와서
    vMtrlDiffuse.a = 1.f;
    
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal, In.vNormal.xyz);
    
    vNormal = mul(vNormal, WorldMatrix);
    
   
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);
    
    return Out;    
}

PS_OUT_TANK PS_BASECOLOR(PS_IN In)
{
    PS_OUT_TANK Out;
    
    vector vMtrlDiffuse = g_vBaseColor * g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
   
    //노말 맵핑을 적용
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    //0~1 사이의 값을 -1~1로 변환한다.
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    //월드매트릭스 상에 있는 픽셀의 노말을 노말맵에 곱한다.
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal, In.vNormal.xyz);
    vNormal = mul(vNormal, WorldMatrix);
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);
    Out.vDepthOutline = Out.vDepth;
    
    return Out;
}

PS_OUT_SKY PS_SKY(PS_IN In)
{
    PS_OUT_SKY Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = In.vNormal;

    return Out;
}

PS_OUT_OUTLINE PS_OUTLINE(PS_IN_OUTLINE In)
{
    PS_OUT_OUTLINE Out;
    Out.vOutline = float4(1.f, 0.f, 0.f, 1.f);
    //Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);
    
    return Out;
}

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
    vector vShadow : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
    PS_OUT_SHADOW Out;
    
    Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);
    
    return Out;
}

// === 테크닉 ===
technique11 DefaultTechnique
{
    //0
    pass Default 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    //1
    pass Sky 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SKY();
    }
    //2
    pass Tank
    {
        SetRasterizerState(RS_Cull_Front);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BASECOLOR();
    }

    //3
    pass Outline
    {
        SetRasterizerState(RS_Cull_Front);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_OUTLINE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_OUTLINE();
    }

    //4
    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }
   
}
