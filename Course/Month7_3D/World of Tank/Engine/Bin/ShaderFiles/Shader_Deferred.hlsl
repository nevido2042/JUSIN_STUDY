#include "Engine_Shader_Defines.hlsli"

/* 상수테이블 ConstantTable */
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv;
matrix g_LightViewMatrix, g_LightProjMatrix;

//float g_fFar;

Texture2D g_RenderTargetTexture;
Texture2D g_NormalTexture;
Texture2D g_DiffuseTexture;
Texture2D g_ShadeTexture;
Texture2D g_DepthTexture;
Texture2D g_SpecularTexture;
Texture2D g_OutlineTexture;
Texture2D g_OutlineDepthTexture;
Texture2D g_ShadowTexture;

vector g_vLightDir;
vector g_vLightPos;
float g_fLightRange;
vector g_vLightDiffuse;
float  g_fLightAmbient;
vector g_vLightSpecular;

float  g_fMtrlAmbient = 0.4f;
vector g_vMtrlSpecular = 1.f;

vector g_vCamPosition;

struct VS_IN
{
    float3 vPosition : POSITION;
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
    
    matrix matWV, matWVP;
    
    /* mul : 모든 행렬의 곱하기를 수행한다. /w연산을 수행하지 않는다. */
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
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
    vector vBackBuffer : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT Out;
    
    Out.vBackBuffer = g_RenderTargetTexture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

struct PS_OUT_LIGHT
{
    vector vShade : SV_TARGET0;
    vector vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out;
    
    //터레인 스페큘러 빼려고 뎁스 기록안했는데도 스페큘러가 고정되서 나오는 문제 발생.
    //이유는 기록된 노말로 스페큘러를 계산하기 때문이었음.
    
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_fLightAmbient * g_fMtrlAmbient);
    
    Out.vShade = g_vLightDiffuse * saturate(fShade);
    
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 500.f;
    
    vector vWorldPos;
    
    /* 로컬위치 * 월드 *뷰 * 투영 / w : 투영공간상의 위치. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
     /* 로컬위치 * 월드 *뷰 * 투영  */
    vWorldPos = vWorldPos * fViewZ;
    
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vReflect = reflect(normalize(g_vLightDir), vNormal);
    vector vLook = vWorldPos - g_vCamPosition;
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 1000.f);
    
    return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out;
    
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    
    float4 vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    
        
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 500.f;
    
    vector vWorldPos;
    
    /* 로컬위치 * 월드 *뷰 * 투영(CLIP) / w : 투영공간상(NDC)의 위치. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
     /* 로컬위치 * 월드 *뷰 * 투영  */
    vWorldPos = vWorldPos * fViewZ;
    
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLightDir = vWorldPos - g_vLightPos;
    
    float fAtt = (g_fLightRange - length(vLightDir)) / g_fLightRange;
    
    float fShade = max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_fLightAmbient * g_fMtrlAmbient);
    
    Out.vShade = g_vLightDiffuse * saturate(fShade) * fAtt;
    
    vector vReflect = reflect(normalize(vLightDir), vNormal);
    vector vLook = vWorldPos - g_vCamPosition;
    
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 50.f) * fAtt;
    
    
    return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    PS_OUT Out;

    vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

    //Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);
    //Out.vDepthOutline = Out.vDepth;
    vector vOutlineDepth = g_OutlineDepthTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vOutline = g_OutlineTexture.Sample(DefaultSampler, In.vTexcoord);

    vector vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexcoord);

    vector vColor = vDiffuse * vShade + vSpecular;

    if (vOutlineDepth.y/*(Near/Far ~ 1)*/ >= 1.f)//깊이값이 없거나, 해당 픽셀이 비어있다(깊이 텍스처에 아무것도 안 찍혔으면)
    {
        vColor += vOutline;
    }

    if (vColor.a == 0.f)
        discard;

    Out.vBackBuffer = vColor;
    
    
    /* 투영상의 z는 0 ~ far, w는 near에서 far까지, 그러니까 vDepth의 x에는 0 ~ 1까지의 값이 참 ( 깊이 ) */
    /* 그리고 월드까지의 변환을 해줄 때, 필요한 w값을 구해주되, 플래그를 UNORM으로 받아왔기 때문에 0 ~ 1로 치환 해주어야 함 */
    
    //그림자
    //Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);
    //In.vProjPos.z(0~Far) / In.vProjPos.w(Near~Far) -> 투영 스페이스의 깊이(0~1)
    //In.vProjPos.w(Near~Far) / 500.0f(Far) -> (Near/Far ~ 1)
    
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 500.f; //(Near~Far)
    
    vector vPosition;

    vPosition.x = In.vTexcoord.x * 2.f - 1.f;
    vPosition.y = In.vTexcoord.y * -2.f + 1.f;
    vPosition.z = vDepthDesc.x;//투영 스페이스의 깊이(0~1)
    vPosition.w = 1.f;

    vPosition = vPosition * fViewZ; //w나누던 연산을 역으로 곱하는 부분
    
    vPosition = mul(vPosition, g_ProjMatrixInv); //투영 역행렬
    vPosition = mul(vPosition, g_ViewMatrixInv); //뷰 역행렬
    
    //월드로 왔음
    
    vPosition = mul(vPosition, g_LightViewMatrix); //빛(카메라) 행렬
    vPosition = mul(vPosition, g_LightProjMatrix); //빛(카메라) 투영 행렬
    
    float2 vTexcoord;
    
    /* (-1, 1 ~ 1, -1) -> (0, 0 ~ 1, 1) */
    vTexcoord.x = vPosition.x / vPosition.w * 0.5f + 0.5f;
    vTexcoord.y = vPosition.y / vPosition.w * -0.5f + 0.5f;
    
    float4 vOldDepthDesc = g_ShadowTexture.Sample(DefaultSampler, vTexcoord);
    float fOldViewZ = vOldDepthDesc.y * 500.f;//(Near~Far)
    
    if (fOldViewZ + 0.f < vPosition.w/*(Near~Far)*/)
        Out.vBackBuffer = Out.vBackBuffer * 0.5f;

    return Out;
}




technique11 DefaultTechnique
{
    pass Debug 
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass Light_Directional
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
    }

    pass Light_Point
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
    }

    pass Deferred
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
    }
  
}
