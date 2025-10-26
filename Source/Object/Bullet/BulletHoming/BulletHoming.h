#pragma once

#include "Object/Bullet/Bullet.h"

class BulletHoming : public Bullet
{
public:
    BulletHoming();
    ~BulletHoming() override {}

    void Initialize(const DirectX::XMFLOAT2& generatePosition);
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

    void Launch(const DirectX::XMFLOAT2& moveDirection, const int& power);

private:
    void Pursuit(const float& elapsedTime);

    const DirectX::XMFLOAT2 CalcNewMoveDirection(const DirectX::XMFLOAT2& direction0, const DirectX::XMFLOAT2& direction1);

private:
    DirectX::XMFLOAT2   moveDirection_ = {};
    float               moveSpeed_ = 500.0f;

    int power_ = 3;

    int state_ = 0;
};