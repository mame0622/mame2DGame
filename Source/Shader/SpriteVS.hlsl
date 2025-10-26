#include "Sprite.hlsli"

PSIn main(VSIn vsIn)
{
    PSIn vsOut;
    
    vsOut.position_ = vsIn.position_;
    vsOut.color_    = vsIn.color_;
    vsOut.texcoord_ = vsIn.texcoord_;
    
    return vsOut;
}