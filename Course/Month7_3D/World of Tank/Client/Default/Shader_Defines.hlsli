

RasterizerState RS_Default
{
    FillMode = solid;
    CullMode = back;
};

RasterizerState RS_Front
{
    FillMode = solid;
    CullMode = back;
};

BlendState BS_Default
{

};

DepthStencilState DSS_Default
{

};

DepthStencilState DSS_NoDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
};