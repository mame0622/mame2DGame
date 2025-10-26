#include "BulletOrbit.h"

BulletOrbit::BulletOrbit()
    : Bullet("BulletOrbit", BulletLevel::One,
        Collision::Type::BulletOrbit, this, 30.0f)
{
}

// ‰Šú‰»
void BulletOrbit::Initialize(const DirectX::XMFLOAT2& bulletSize)
{
    GetTransform()->SetSize(bulletSize);
    GetTransform()->SetTexSize(bulletSize);
    GetTransform()->SetPivot(bulletSize * 0.5f);
}

// XV
void BulletOrbit::Update(const float& elapsedTime)
{
    const float alpha = IsCollisionActive() ? 1.0f : 0.0f;
    GetTransform()->SetColorA(alpha);
}

// ImGui
void BulletOrbit::DrawDebug()
{
}

// Õ“ËŒŸ’m
void BulletOrbit::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
}
