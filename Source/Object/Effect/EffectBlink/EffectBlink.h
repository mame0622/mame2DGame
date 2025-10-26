#pragma once
#include "Object/Effect/Effect.h"

class EffectBlink : public Effect
{
public:
    EffectBlink();
    ~EffectBlink() override {}

    void Initialize(const DirectX::XMFLOAT2& centerPosition, const float& minSize, const float& maxSize);
    void Update(const float& elapsedTime);

private:
    DirectX::XMFLOAT2 centerPosition_ = {};
    
    float minSize_ = 30.0f;
    float maxSize_ = 70.0f;

    float alpha_ = 1.0f;
};

