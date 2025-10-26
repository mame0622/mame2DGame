#include "CollisionManager.h"
#include "Object/Character/Player/PlayerManager.h"
#include "Object/Character/Enemy/EnemyManager.h"
#include "Object/Character/Enemy/Enemy.h"
#include "Object/Bullet/BulletManager.h"
#include "Application/Common.h"

// 更新
void CollisionManager::Update()
{
    // 生成処理
    for (Collision* collision : generates_)
    {
        collisions_.emplace_back(collision);
    }
    generates_.clear();

    // 削除処理
    for (Collision* collision : removes_)
    {
        auto it = std::find(collisions_.begin(), collisions_.end(), collision);

        if (it != collisions_.end()) collisions_.erase(it);

        SafeDeletePtr(collision);
    }
    removes_.clear();

    // 更新
    for (int i = 0; i < collisions_.size(); ++i)
    {
        for (int j = i + 1; j < collisions_.size(); ++j)
        {
            auto collision1 = collisions_.at(i);
            auto collision2 = collisions_.at(j);

            if (collisionMatrix[(int)collision1->GetType()][(int)collision2->GetType()] == false)
            {
                continue;
            }
      
            DirectX::XMFLOAT2 resultPosition = {};
            if (IntersectCircleVsCircle(
                collision1->GetTransform()->GetCenterPosition(), collision1->GetRadius(),
                collision2->GetTransform()->GetCenterPosition(), collision2->GetRadius(),
                collision2->GetTransform()->GetSize(), resultPosition))
            {
                collision1->OnHit(collision2->GetType(), {});
                collision2->OnHit(collision1->GetType(), resultPosition);
            }
        }
    }
}

// 円 Vs 円
const bool CollisionManager::IntersectCircleVsCircle(const DirectX::XMFLOAT2& positionA, const float& radiusA, const DirectX::XMFLOAT2& positionB, const float& radiusB, const DirectX::XMFLOAT2& sizeB, DirectX::XMFLOAT2& resultPosition)
{
    const DirectX::XMFLOAT2 vec = positionB - positionA;
    const float length = XMFloat2Length(vec);
    const float overlap = radiusA + radiusB - length;

    // 当たっていない
    if (overlap <= 0) return false;

    const DirectX::XMFLOAT2 offset = sizeB * 0.5f;
    resultPosition = positionB + XMFloat2Normalize(vec) * overlap - offset;

    return true;
}