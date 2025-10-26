#include "EnemyPlanaria.h"
#include "Object/Character/Player/PlayerManager.h"
#include "Object/Character/Enemy/EnemyNormal/EnemyNormal.h"


EnemyPlanaria::EnemyPlanaria()
    :Enemy("EnemyPlanaria", EnemyManager::EnemyType::Planaria,
        Collision::Type::Enemy, this, 50.0f)
{
}

// 初期化
void EnemyPlanaria::Initialize()
{
    const float size = 100.0f;
    GetTransform()->SetSize(size);
    GetTransform()->SetTexSize(size);
    GetTransform()->SetPivot(size * 0.5f);

    SetHealth(3);
}

// 更新処理
void EnemyPlanaria::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    // 追跡処理
    Pursuit(elapsedTime);

    if(GetHealth() <= 0)
    {
        // 分裂体の数
        const int numberOfSplits = 3;

        for (int i = 0; i < numberOfSplits; ++i)
        {
            EnemyNormal* enemy = new EnemyNormal();
            // 生成位置設定
            enemy->GetTransform()->SetPositionX(GetTransform()->GetPosition().x + i);
            enemy->GetTransform()->SetPositionY(GetTransform()->GetPosition().y);
        }
        // 破棄
        EnemyManager::Instance().Remove(this);
    }
}

// ImGui
void EnemyPlanaria::DrawDebug()
{
}

// 衝突検知
void EnemyPlanaria::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
    // 押し出し判定
    if (type == Collision::Type::Enemy || type == Collision::Type::BulletOrbit)
    {
        if (position.x == 0.0f && position.y == 0.0f) return;

        GetTransform()->SetPosition(position);
    }

    if (type == Collision::Type::Bullet)
    {
        Damage(1);
    }
}

// 追跡処理
void EnemyPlanaria::Pursuit(const float& elapsedTime)
{
    const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();
    const DirectX::XMFLOAT2 enemyCenterPosition = GetTransform()->GetCenterPosition();
    const DirectX::XMFLOAT2 moveVec = XMFloat2Normalize(playerCenterPosition - enemyCenterPosition);

    GetTransform()->AddPosition(moveVec * moveSpeed_ * elapsedTime);
    GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(moveVec.y, moveVec.x) + DirectX::XM_PIDIV2));
}
