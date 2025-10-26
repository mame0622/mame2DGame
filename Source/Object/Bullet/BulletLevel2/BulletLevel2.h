#pragma once
#include "Object/Bullet/Bullet.h"

class BulletLevel2 : public Bullet
{
public:
    BulletLevel2();
    ~BulletLevel2() override {}

    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void Launch(const int& bulletNumber, const DirectX::XMFLOAT2& direction, const float& length);

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;
};

