struct VSIn
{
    float4 position_    : POSITION;
    float4 color_       : COLOR;
    float2 texcoord_    : TEXCOORD;
};

struct PSIn
{
    float4 position_    : SV_POSITION;
    float4 color_       : COLOR;
    float2 texcoord_    : TEXCOORD;
};