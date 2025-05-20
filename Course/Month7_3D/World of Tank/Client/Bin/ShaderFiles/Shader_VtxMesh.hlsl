
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap;
    AddressV = wrap;

};

// === UV Offset / ȸ���� ������� �߰� ===
float2  g_UVOffset = float2(0.f, 0.f); // �ִϸ��̼ǿ� offset
//float   g_UVRotation = 0.f; // ���� ���� ȸ����
//int     g_IsTread = 0; // �˵� ����

// === ���� ����ü ===
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

// === UV ȸ�� �Լ� ===
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

// === ���� ���̴� ===
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : ��� ����� ���ϱ⸦ �����Ѵ�. /w������ �������� �ʴ´�. */
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

// === �ȼ� ���̴� ===
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

// === ��ũ�� ===
technique11 DefaultTechnique
{
   
    pass Default
    {
        VertexShader = compile vs_5_0 VS_MAIN();        
        PixelShader = compile ps_5_0 PS_MAIN();      
    }
   
}
