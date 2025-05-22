matrix  g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D   g_DiffuseTexture;

float4 g_vLightDir;
float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vLightSpecular;

float4 g_vMtrlAmibient = float4(0.5f, 0.5f, 0.5f, 1.f);

//float2  g_TexOffset; // UV 오프셋
//float2  g_TexScale; // UV 타일링 배수 (예: 3.0, 3.0)

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap; // 반복 모드
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
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    matrix matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matrix matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
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
    
    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * vShade;
    
    // 3x3 타일링 및 오프셋 적용
    //float2 tiledUV = In.vTexcoord * g_TexScale + g_TexOffset;
    
    // AddressU/V가 wrap이므로 tiledUV가 0~1 범위 넘어가도 자동 반복됨
    //Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, tiledUV);

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
