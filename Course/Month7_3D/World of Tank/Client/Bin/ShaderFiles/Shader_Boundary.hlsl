#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

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

    // TransformMatrix�� �̿��� ��ġ ���
    vector vWorldPos = mul(vector(In.vPosition, 1.f), In.TransformMatrix);
    Out.vPosition = mul(vWorldPos, g_WorldMatrix);

    // ������ ���
    Out.vPSize = float2(length(In.TransformMatrix._11_12_13), length(In.TransformMatrix._21_22_23));

    // ���� ���� ���� (GS���� billboard ��� �� �ϰ� �ϱ� ����)
    Out.vRight = normalize(In.TransformMatrix._11_12_13);
    Out.vUp = normalize(In.TransformMatrix._21_22_23);

    return Out;
}

/* �׸��� ���¿� ���� ȣ��ȴ�. */ 
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
    
    // y�� �������� �Ʒ�(0.0) ��(1.0)�ϼ��� ���ϰ�
    float alpha = lerp(0.9f, 0.05f, 1.f - In.vTexcoord.y); // �Ʒ�: ����, ��: ����

    Out.vColor = float4(1.f, 0.f, 0.f, alpha);
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
 
}
