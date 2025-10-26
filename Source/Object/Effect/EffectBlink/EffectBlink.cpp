#include "EffectBlink.h"

EffectBlink::EffectBlink()
    : Effect("EffectBlink", EffectManager::EffectType::Blink)
{
}

void EffectBlink::Initialize(const DirectX::XMFLOAT2& centerPosition, const float& minSize, const float& maxSize)
{
    centerPosition_ = centerPosition;
    minSize_ = minSize;
    maxSize_ = maxSize;
}

// 更新
void EffectBlink::Update(const float& elapsedTime)
{
    alpha_ -= elapsedTime;
    if (alpha_ <= 0.0f) alpha_ = 0.0f;
    
    // 透明度
    GetTransform()->SetColorA(alpha_ + 0.3f);
    
    // 大きさ
    const float size = XMFloatLerp(maxSize_, minSize_, alpha_);
    GetTransform()->SetSize(size);
    GetTransform()->SetTexSize(size);
    GetTransform()->SetPivot(size * 0.5f);

    // 位置
    const DirectX::XMFLOAT2 position = centerPosition_ - DirectX::XMFLOAT2(size, size) * 0.5f;
    GetTransform()->SetPosition(position);

    if (alpha_ <= 0.0f)
    {
        EffectManager::Instance().Remove(this);
    }
}