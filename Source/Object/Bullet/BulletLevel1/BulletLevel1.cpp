#include "BulletLevel1.h"
#include "Object/Bullet/BulletManager.h"
#include "ImGui/ImGuiCtrl.h"
#include "Object/Character/Player/PlayerManager.h"

BulletLevel1::BulletLevel1()
    : Bullet("BulletLevel1", BulletLevel::One,
        Collision::Type::Bullet, this, 25.0f)
{
    //const float bulletSize = 30.0f;
    // Bulletのサイズ設定
    //GetTransform()->SetSize(bulletSize);
    //GetTransform()->SetTexSize(bulletSize);
    //GetTransform()->SetPivot(bulletSize * 0.5f);
}

// 更新
void BulletLevel1::Update(const float& elapsedTime)
{
    Bullet::Update(elapsedTime);

    GetTransform()->AddPosition(moveDirection_ * moveSpeed_ * elapsedTime);
}

// ImGui
void BulletLevel1::DrawDebug()
{
#if USE_IMGUI
    const std::string name = GetName() + std::to_string(GetObjectId());
    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed))
    {
        ImGui::DragFloat("MoveSpeed", &moveSpeed_);

        ImGui::TreePop();
    }

#endif // USE_IMGUI
}

// 発射
void BulletLevel1::Launch(const int& bulletNumber, const DirectX::XMFLOAT2& direction, const float& length)
{
    // BulletNumber設定
    SetBulletNumber(bulletNumber);

    // 進行方向設定
    moveDirection_ = direction;

    // 生成位置設定
    const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();
    const DirectX::XMFLOAT2 offsetPosition = GetTransform()->GetSize() * 0.5f;
    const DirectX::XMFLOAT2 createPosition = playerCenterPosition + direction * length - offsetPosition;
    GetTransform()->SetPosition(createPosition);

    // 角度設定
    GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(direction.y, direction.x) + DirectX::XM_PIDIV2));
}

// 衝突検知
void BulletLevel1::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
    if (type == Collision::Type::Enemy)
    {
        BulletManager::Instance().GetOrbitBullet(GetBulletNumber())->SetLevel(BulletLevel::Two);
        BulletManager::Instance().GetOrbitBullet(GetBulletNumber())->SetCollisionActive(true);

        // 分からん
        CollisionRemove();
        BulletManager::Instance().Remove(this);
    }
}
