#include "Engine_Shader_Defines.hlsli"

/* ������̺� ConstantTable */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;

float g_fAlpha = 0.3f;

float g_fFill = 1.f;
float g_fFillDelay = 1.f;
float g_fFillDelayValue = 0.f;

float4 g_vBaseColor = float4(1.f, 1.f, 1.f, 1.f);

float2 g_vAimCenter = float2(0.5f, 0.5f); // ȭ�� �߽� (UV ����)
float g_fAimRadius = 0.01f; // ���ؿ� ������ (0~0.5)
float g_fLineWidth = 0.0005f; // ���ؼ� �β�
float4 g_vAimColor = float4(0.f, 1.f, 0.f, 1.f); // ���
float g_fZoomScale = 1.0f; // 1.0 (�⺻ ��), �۾����� ����

//����Ʈ ����Ʈ��
Texture2D g_DepthTexture;



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

struct VS_OUT_BLEND
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    
};

VS_OUT_BLEND VS_MAIN_BLEND(VS_IN In)
{
    VS_OUT_BLEND Out;
    
    matrix matWV, matWVP;
    
    /* mul : ��� ����� ���ϱ⸦ �����Ѵ�. /w������ �������� �ʴ´�. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    
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
    
    return Out;
}

PS_OUT PS_DAMAGELOG(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb = float3(0.f, 1.f, 0.f);
    Out.vColor.a *= 4.f;
    
    return Out;
}

PS_OUT PS_ALPHATESTA(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    if (Out.vColor.a < 0.3f)
        discard;
    
    return Out;
}

PS_OUT PS_BASECOLOR(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_vBaseColor * g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

PS_OUT PS_FILL_COLOR(PS_IN In)
{
    PS_OUT Out;

    if (In.vTexcoord.x >= g_fFill)
        discard;

    float4 texColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);

    Out.vColor = lerp(texColor, float4(1.f - g_fFill, g_fFill, 0.0, texColor.a), 0.7); // ������ 30% ����

    return Out;
}

PS_OUT PS_DAMAGEBAR_WORLD(PS_IN In)
{
    PS_OUT Out;
    
    float4 texColor;
    
    float4 dark = g_Texture.Sample(DefaultSampler, In.vTexcoord) * float4(0.f, 0.f, 0.f, 0.5f);

    if (In.vTexcoord.x <= g_fFill)
    {
        //���� ü��
        texColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    }
    else if (In.vTexcoord.x <= g_fFillDelay)
    {
        texColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
        
        float4 yellow = float4(1.f, 1.f, 0.f, texColor.a);
        // �ð� ������� ���� ��ο����� ��ο� �� ��ȯ
        texColor = lerp(yellow, dark, g_fFillDelayValue);

    }
    else
    {
        //��ο� ���
        texColor = dark;
    }

    Out.vColor = texColor;

    return Out;
}

PS_OUT PS_AIMCIRCLE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0; //�ʱ�ȭ

    float2 uv = In.vTexcoord;
    float scale = g_fZoomScale;

    float radius = g_fAimRadius * scale;
    float width = g_fLineWidth;
    float dist = distance(uv, g_vAimCenter);

    if (abs(dist - radius) < width)
        Out.vColor = g_vAimColor;
    else
        discard;

    return Out;
}

struct PS_IN_BLEND
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};


PS_OUT PS_MAIN_BLEND(PS_IN_BLEND In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
   
    /*ȭ�� ��ü ����(0, 0 ~ 1, 1)���� ����Ʈ�� �ȼ��� �׷��� ��ġ�� �ش��ϴ� ��ǥ */    
    float2 vTexcoord;
    
    /*����Ʈ�� Ư�� �ȼ�(psin)�� ȭ�� ��ü�������� ��� �����ϴ���? */ 
    /* �켱 ����������(-1, 1 -> 1, -1)�� �ȼ��� ��ġ�� ���Ѵ�.*/    
    vTexcoord.x = In.vProjPos.x / In.vProjPos.w;
    vTexcoord.y = In.vProjPos.y / In.vProjPos.w;
    
    vTexcoord.x = vTexcoord.x * 0.5f + 0.5f;
    vTexcoord.y = vTexcoord.y * -0.5f + 0.5f;
    
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexcoord);
     
    //if (vDepthDesc.y == 0.f)
    //    return Out;

    float fOldViewZ = vDepthDesc.y * 1000.f;
    float fRealDistance = (fOldViewZ /*�̰͵� ��¥ ī�޶���� ���� �Ÿ�*/ - In.vProjPos.w/*�̰� ��¥ ī�޶� ���� �Ÿ�*/); 
    
    Out.vColor.a = Out.vColor.a * (saturate(fRealDistance));
   
    return Out;
}

PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = float4(1.f, 0.8f, 0.f, 1.f);
    
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
    pass AlphaBlend/* ��� + ����ŧ�� + �׸��� + ssao + ������Ʈ */ 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();        
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    //1
    pass FILL
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FILL_COLOR();
    }
    //2
    pass Effect
    {
        SetRasterizerState(RS_Cull_Front);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_BLEND();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND();
    }
    //3
    pass BaseColor
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BASECOLOR();
    }
    //4
    pass DamageBar_World
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DAMAGEBAR_WORLD();
    }

    // 5
    pass AimCircle
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0, 0, 0, 0), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_AIMCIRCLE();
    }

    //6
    pass Trail
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0, 0, 0, 0), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }
    //7
    pass Button
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ALPHATESTA();
    }
    //8
    pass DamageLog
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DAMAGELOG();
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
