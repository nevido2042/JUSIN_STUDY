#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_Texture;
vector g_vCamPosition;

struct VS_IN
{
    float3 vPosition : POSITION;       
    
    row_major float4x4 TransformMatrix : WORLD;  
};

struct VS_OUT_BOUNDARY
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;

    float3 vRight : TEXCOORD1;
    float3 vUp : TEXCOORD2;
};

VS_OUT_BOUNDARY VS_BOUNDARY(VS_IN In)
{
    VS_OUT_BOUNDARY Out;

    // TransformMatrix를 이용한 위치 계산
    vector vWorldPos = mul(vector(In.vPosition, 1.f), In.TransformMatrix);
    Out.vPosition = mul(vWorldPos, g_WorldMatrix);

    // 사이즈 계산
    Out.vPSize = float2(length(In.TransformMatrix._11_12_13), length(In.TransformMatrix._21_22_23));

    // 방향 벡터 전달
    Out.vRight = normalize(In.TransformMatrix._11_12_13);
    Out.vUp = normalize(In.TransformMatrix._21_22_23);

    return Out;
}

/* 그리는 형태에 따라서 호출된다. */ 
struct GS_IN_BOUNDARY
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;

    float3 vRight : TEXCOORD1;
    float3 vUp : TEXCOORD2;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN_BOUNDARY In[1], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];
    
    //float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    
    float3 vLook = g_vCamPosition.xyz - In[0].vPosition.xyz;
    vLook.y = 0.f; // Y축 고정
    vLook = normalize(vLook); // 정규화 다시 해줌
    
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
    float3 vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    
    Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    
    Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    
    Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    
    Triangles.Append(Out[0]);
    Triangles.Append(Out[1]);
    Triangles.Append(Out[2]);
    Triangles.RestartStrip();
    
    Triangles.Append(Out[0]);
    Triangles.Append(Out[2]);
    Triangles.Append(Out[3]);
    Triangles.RestartStrip();
}

[maxvertexcount(6)]
void GS_BOUNDARY(point GS_IN_BOUNDARY In[1], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];

    float3 vRight = normalize(In[0].vRight.xyz) * In[0].vPSize.x * 0.5f;
    float3 vUp = normalize(In[0].vUp.xyz) * In[0].vPSize.y * 0.5f;

    float3 vCenter = In[0].vPosition.xyz;

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = mul(float4(vCenter + vRight + vUp, 1.f), matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);

    Out[1].vPosition = mul(float4(vCenter - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);

    Out[2].vPosition = mul(float4(vCenter - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);

    Out[3].vPosition = mul(float4(vCenter + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);

    Triangles.Append(Out[0]);
    Triangles.Append(Out[1]);
    Triangles.Append(Out[2]);
    Triangles.RestartStrip();

    Triangles.Append(Out[0]);
    Triangles.Append(Out[2]);
    Triangles.Append(Out[3]);
    Triangles.RestartStrip();
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_BOUNDARY(PS_IN In)
{
    PS_OUT Out;
    
    // y축 기준으로 아래(0.0) 위(1.0)일수록 연하게
    float alpha = lerp(0.9f, 0.05f, 1.f - In.vTexcoord.y); // 아래: 진함, 위: 연함

    Out.vColor = float4(1.f, 0.f, 0.f, alpha);
    return Out;
}

PS_OUT PS_VEGETATION(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (Out.vColor.a < 0.3f)
        discard;
    
    return Out;
}



technique11 DefaultTechnique
{
    pass Boundary//0
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_BOUNDARY();
        GeometryShader = compile gs_5_0 GS_BOUNDARY();
        PixelShader = compile ps_5_0 PS_BOUNDARY();
    }
    pass Vegetation //1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_BOUNDARY();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_VEGETATION();
    }
 
}
