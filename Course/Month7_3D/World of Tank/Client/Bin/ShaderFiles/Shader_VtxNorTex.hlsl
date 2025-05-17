matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
float2 g_TexOffset; // UV ������
float2 g_TexScale; // UV Ÿ�ϸ� ��� (��: 3.0, 3.0)

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap; // �ݺ� ���
    AddressV = wrap;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    matrix matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matrix matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
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

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    // 3x3 Ÿ�ϸ� �� ������ ����
    float2 tiledUV = In.vTexcoord * g_TexScale + g_TexOffset;

    // AddressU/V�� wrap�̹Ƿ� tiledUV�� 0~1 ���� �Ѿ�� �ڵ� �ݺ���
    Out.vColor = g_Texture.Sample(DefaultSampler, tiledUV);

    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
