#include "Engine_Shader_Defines.hlsli"

/* ������̺� ConstantTable */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
float g_fAlpha = 0.3f;

float g_fFill = 1.f;

/* ������ �������� ��ȯ (���庯ȯ, ��, ������ȯ) */ 
/* ������ ���� ������ ������ �� �ִ�. */ 

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    /* SV_ : ShaderValue���� */
    /* ���� �ؾ��� ������ �� ������ ���� ��(��ġ)�� �˾Ƽ� �߰����� ������ �ض�. */     
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : ��� ����� ���ϱ⸦ �����Ѵ�. /w������ �������� �ʴ´�. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;    
    
    return Out;
}

//POSITION�ø�ƽ�� ����
//��������� ���ؼ�

/* W������ ������ �����Ѵ�. */
/* ����Ʈ�� ��ȯ�Ѵ�. */
/* ��� ���п� ���ؼ� �����Ͷ�����. -> �ȼ��� �����Ѵ�. */


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
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (Out.vColor.a < g_fAlpha)
        discard;
    
    return Out;
}

PS_OUT PS_FILL(PS_IN In)
{
    PS_OUT Out;

    if (In.vTexcoord.x >= g_fFill)
        discard;

    float4 texColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    //if (texColor.a < g_fAlpha)
    //    discard;
    
    Out.vColor = lerp(texColor, float4(1.f - g_fFill, g_fFill, 0.0, texColor.a), 0.7); // ������ 30% ����

    return Out;
}



technique11 DefaultTechnique
{
    /* �н��� �����ϴ� ������ ����? */ 
    /* ���� ���� �׸��� ���� �ٸ� �������������� �Կ����ϰų�. 
    ������ �ٸ� ���̵� ����� �����ؾ��ϰų� */ 

    /* �� ���̴��� � �������� �����Ұ���.  */ 
    /* � ���̴��� ����Ұ���? */ 
    /* ������ �Լ� ���� */
    /* �������������� ���� ����*/ 

    //0
    pass Default/* ��� + ����ŧ�� + �׸��� + ssao + ������Ʈ */ 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();        
        PixelShader = compile ps_5_0 PS_MAIN();      
    }
    //1
    pass FILL
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_FILL();
    }
    //2
    pass GUN_MARKER
    {
        SetRasterizerState(RS_Cull_Front);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }



    //pass Disstortion/* �ְ� 1*/ 
    //{
    //    VertexShader = compile vs_5_0 VS_MAIN1();
    //    PixelShader = compile ps_5_0 PS_MAIN_Diss();
    //}
    //pass Blend/* ������ */
    //{
    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    PixelShader = compile ps_5_0 PS_MAIN_Blend();
    //}
}
