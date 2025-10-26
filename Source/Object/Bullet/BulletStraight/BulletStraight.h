#pragma once
#include "Object/Bullet/Bullet.h"

class BulletStraight : public Bullet
{
public:
    BulletStraight();
    ~BulletStraight() override {}

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

    void Launch(const DirectX::XMFLOAT2& generatePosition, const DirectX::XMFLOAT2& moveDirection);

    
private:
    float alpha_ = 0;
};

