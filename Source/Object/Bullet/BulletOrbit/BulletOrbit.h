#pragma once
#include "Object/Bullet/Bullet.h"

class BulletOrbit : public Bullet
{
public:
    BulletOrbit();
    ~BulletOrbit() override {}

    void Initialize(const DirectX::XMFLOAT2& bulletSize);
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

    void SetCollisionActive(const bool& flag) { isCollisionActive_ = flag; }
    const bool IsCollisionActive() const { return isCollisionActive_; }

private:
    bool isCollisionActive_ = true;
};

