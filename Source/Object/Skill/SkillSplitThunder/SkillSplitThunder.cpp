#include "SkillSplitThunder.h"
#include "Graphics/Graphics.h"
#include "Collision/CollisionManager.h"

SkillSplitThunder::SkillSplitThunder()
    : Skill("SplitThunder", SkillManager::SkillType::SkillSplitThunder),
    collision_(new Collision(Collision::Type::Bullet, this, 30.0f))
{
}

// 更新
void SkillSplitThunder::Update(const float& elapsedTime)
{
    Transform2D& transform = transforms_.at(0);

    // 移動処理
    transform.AddPosition(moveDirection_ * moveSpeed_ * elapsedTime);

    // 画面外に出たら自身を消去する
    const DirectX::XMFLOAT2 minPosition = transform.GetSize() * -1.0f;
    const DirectX::XMFLOAT2 maxPosition = DirectX::XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
    const DirectX::XMFLOAT2 position = transform.GetPosition();

    if (position.x <= minPosition.x || position.y <= minPosition.y ||
        position.x >= maxPosition.x || position.y >= maxPosition.y)
    {
        SkillManager::Instance().Remove(this);
        
        // Collisionの後処理
        CollisionRemove();
    }

    // ObjectにあるTransformを更新する※当たり判定用
    Object::GetTransform()->SetPosition(transform.GetPosition());
}

// ImGui
void SkillSplitThunder::DrawDebug()
{
}
// 発射
void SkillSplitThunder::Launch(const int& splitCount, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& oldDirection, const float& angle)
{
    Transform2D transform = {};
    transform.SetPosition(position);
    transform.SetSize(50.0f);
    transform.SetTexSize(50.0f);
    transform.SetPivot(25.0f);

    transforms_.emplace_back(transform);

    // ObjectにあるTransformを設定する※当たり判定用
    Object::GetTransform()->SetPosition(transform.GetPosition());
    Object::GetTransform()->SetSize(transform.GetSize());
    Object::GetTransform()->SetTexSize(transform.GetTexSize());
    Object::GetTransform()->SetPivot(transform.GetPivot());

    // 進行方向を決定する
    moveDirection_ =
    {
        oldDirection.x * cosf(angle) - oldDirection.y * sinf(angle),
        oldDirection.x * sinf(angle) + oldDirection.y * cosf(angle)
    };

    splitCount_ = splitCount;
}

// 衝突検知
void SkillSplitThunder::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
    if (type == Collision::Type::Enemy)
    {
        ++splitCount_;
        // 上限回数に達した
        if (splitCount_ > maxSplint_)
        {
            // 自分自身の削除
            SkillManager::Instance().Remove(this);
            // Collisionの後処理
            CollisionRemove();

            return;
        }

        // 新たに2個生成する
        SkillSplitThunder* splitThunder0 = new SkillSplitThunder();
        SkillSplitThunder* splitThunder1 = new SkillSplitThunder();
        splitThunder0->Launch(splitCount_, Object::GetTransform()->GetPosition(), moveDirection_, DirectX::XMConvertToRadians(45.0f));
        splitThunder1->Launch(splitCount_, Object::GetTransform()->GetPosition(), moveDirection_, DirectX::XMConvertToRadians(-45.0f));

        // 自分自身の削除
        SkillManager::Instance().Remove(this);
        // Collisionの後処理
        CollisionRemove();
    }
}

// Collisionの後処理
void SkillSplitThunder::CollisionRemove()
{
    CollisionManager::Instance().Remove(collision_);
    collision_ = nullptr;
}