Texture2D colorMap : register(t0);
SamplerState samplerStates[5] : register(s0);

struct PSIn
{
    float4 position_ : SV_POSITION;
    float2 texcoord_ : TEXCOORD;
};

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = colorMap.Sample(samplerStates[1], psIn.texcoord_);
    
    return color;
}