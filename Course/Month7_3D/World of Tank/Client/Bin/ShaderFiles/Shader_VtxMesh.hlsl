
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap;
    AddressV = wrap;

};

// === UV Offset / 회전용 상수버퍼 추가 ===
float2  g_UVOffset = float2(0.f, 0.f); // 애니메이션용 offset
//float   g_UVRotation = 0.f; // 라디안 단위 회전값
//int     g_IsTread = 0; // 궤도 여부

// === 정점 구조체 ===
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
    float2 vTexcoord : TEXCOORD0;    
};

// === UV 회전 함수 ===
float2 RotateUV(float2 uv, float angle)
{
    float2 pivot = float2(0.5f, 0.5f);
    float2 centered = uv - pivot;
    
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    
    float2 rotated;
    rotated.x = centered.x * cosTheta - centered.y * sinTheta;
    rotated.y = centered.x * sinTheta + centered.y * cosTheta;

    return rotated + pivot;
}

// === 정점 셰이더 ===
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    float2 uv = In.vTexcoord;
    //if (g_IsTread == 1)
    //{
        //uv = RotateUV(uv, g_UVRotation);
        uv += g_UVOffset;
    //}

    Out.vTexcoord = uv;
    
    return Out;
}

// === 픽셀 셰이더 ===
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
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    //if(Out.vColor.a < 0.3f)
    //    discard;
    
    return Out;    
}

// === 테크닉 ===
technique11 DefaultTechnique
{
   
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();        
        PixelShader = compile ps_5_0 PS_MAIN();      
    }
   
}
