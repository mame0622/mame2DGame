#include "Sprite.hlsli"
Texture2D colorMap : register(t0);
SamplerState samplerStates[3] : register(s0);

float4 main(PSIn psIn) : SV_TARGET
{
    float4 color = colorMap.Sample(samplerStates[1], psIn.texcoord_);
    
    return color * psIn.color_;
}