#pragma once
#include "Object/Bullet/Bullet.h"

class BulletLevel1 : public Bullet
{
public:
    BulletLevel1();
    ~BulletLevel1() override {}

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void Launch(const int& bulletNumber, const DirectX::XMFLOAT2& direction, const float& length);

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;
};

