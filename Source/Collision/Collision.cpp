#include "Collision.h"
#include "Object/Object.h"
#include "CollisionManager.h"

Collision::Collision(const Type& type, Object* owner, const float& radius)
    : type_(type), owner_(owner), radius_(radius)
{
    CollisionManager::Instance().Register(this);
}

// 自分自身をマネージャーのリストから除外する
void Collision::Remove()
{
    CollisionManager::Instance().Remove(this);
}

void Collision::OnHit(const Type& type, const DirectX::XMFLOAT2& position)
{
    owner_->OnHit(type, position);
}

Transform2D* Collision::GetTransform()
{
    return owner_->GetTransform();
}