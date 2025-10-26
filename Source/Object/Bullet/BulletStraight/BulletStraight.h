#pragma once
#include "Object/Bullet/Bullet.h"

class BulletStraight : public Bullet
{
public:
    BulletStraight();
    ~BulletStraight() override {}

    void Initialize(const DirectX::XMFLOAT2& generatePosition);
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

    void Launch(const DirectX::XMFLOAT2& moveDirection);

    
private:
    DirectX::XMFLOAT2   moveDirection_ = {};
    float               moveSpeed_ = 0;
    float               alpha_ = 0;
};

