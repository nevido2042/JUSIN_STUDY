#include "Engine_Shader_Defines.hlsli"

/* 상수테이블 ConstantTable */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
float g_fAlpha = 0.3f;

float g_fFill = 1.f;
float g_fFillDelay = 1.f;
float g_fFillDelayValue = 0.f;

float4 g_vBaseColor = float4(1.f, 1.f, 1.f, 1.f);

float2 g_vAimCenter = float2(0.5f, 0.5f); // 화면 중심 (UV 기준)
float g_fAimRadius = 0.01f; // 조준원 반지름 (0~0.5)
float g_fLineWidth = 0.0005f; // 조준선 두께
float4 g_vAimColor = float4(0.f, 1.f, 0.f, 1.f); // 흰색
float g_fZoomScale = 1.0f; // 1.0 (기본 줌), 작아지면 줌인

/* 정점의 기초적인 변환 (월드변환, 뷰, 투영변환) */ 
/* 정점의 구성 정보를 변형할 수 있다. */ 

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    /* SV_ : ShaderValue약자 */
    /* 내가 해야할 연산은 다 했으니 이제 니(장치)가 알아서 추가적인 연산을 해라. */     
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;    
    
    return Out;
}

//POSITION시멘틱이 붙은
//멤버변수에 대해서

/* W나누기 연산을 수행한다. */
/* 뷰포트로 변환한다. */
/* 모든 성분에 대해서 래스터라이즈. -> 픽셀을 생성한다. */


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

    Out.vColor = lerp(texColor, float4(1.f - g_fFill, g_fFill, 0.0, texColor.a), 0.7); // 빨간색 30% 섞기

    return Out;
}

PS_OUT PS_DAMAGEBAR_WORLD(PS_IN In)
{
    PS_OUT Out;
    
    float4 texColor;
    
    float4 dark = g_Texture.Sample(DefaultSampler, In.vTexcoord) * float4(0.f, 0.f, 0.f, 0.5f);

    if (In.vTexcoord.x <= g_fFill)
    {
        //현재 체력
        texColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    }
    else if (In.vTexcoord.x <= g_fFillDelay)
    {
        texColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
        
        float4 yellow = float4(1.f, 1.f, 0.f, texColor.a);
        // 시간 기반으로 점점 어두워지는 어두운 색 전환
        texColor = lerp(yellow, dark, g_fFillDelayValue);

    }
    else
    {
        //어두운 배경
        texColor = dark;
    }

    Out.vColor = texColor;

    return Out;
}

PS_OUT PS_AIMCIRCLE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0; //초기화

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


technique11 DefaultTechnique
{
    /* 패스를 생성하는 기준을 뭘로? */ 
    /* 같은 모델을 그릴때 각기 다른 렌더스테이츠를 먹여야하거나. 
    완전히 다른 쉐이딩 기법을 적용해야하거나 */ 

    /* 각 쉐이더를 어떤 버젼으로 빌드할건지.  */ 
    /* 어떤 쉐이더를 사용할건지? */ 
    /* 진입점 함수 결정 */
    /* 렌더스테이츠에 대한 설정*/ 

    //0
    pass Default/* 명암 + 스펙큘러 + 그림자 + ssao + 림라이트 */ 
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
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_FILL_COLOR();
    }
    //2 (버리기 대기중)
    pass UI3D
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
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


    //pass Disstortion/* 왜곡 1*/ 
    //{
    //    VertexShader = compile vs_5_0 VS_MAIN1();
    //    PixelShader = compile ps_5_0 PS_MAIN_Diss();
    //}
    //pass Blend/* 반투명 */
    //{
    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    PixelShader = compile ps_5_0 PS_MAIN_Blend();
    //}
}
