
sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap;
    AddressV = wrap;

};

sampler PointSampler = sampler_state
{
    filter = min_mag_mip_point;
    AddressU = wrap;
    AddressV = wrap;
};

sampler LinearClampSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;
};

RasterizerState RS_Default
{
    FillMode = Solid;
    CullMode = back;
};

RasterizerState RS_Cull_Front
{
    FillMode = Solid;
    CullMode = front;
};

RasterizerState RS_Cull_None
{
    FillMode = Solid;
    CullMode = None;
};

RasterizerState RS_Wireframe
{
    FillMode = Wireframe; // 와이어프레임 모드
    CullMode = None; // 전면/후면 컬링 안 함 (원하는 경우 back 또는 front로 바꿔도 됨)
};


DepthStencilState DSS_Default
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    DepthWriteMask = zero;    
};

BlendState BS_Default
{
    BlendEnable[0] = false;

};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;
    SrcBlend = Src_Alpha;
    DestBlend = Inv_Src_Alpha;
    BlendOp = Add;
};
