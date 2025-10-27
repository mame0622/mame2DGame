#include "EnemyRobot.h"
#include "Object/Character/Player/PlayerManager.h"
#include "Object/Bullet/BulletStraight/BulletStraight.h"
#include "Object/Bullet/BulletHoming/BulletHoming.h"
#include "Object/Bullet/BulletManager.h"
#include "Input/Input.h"

EnemyRobot::EnemyRobot()
    : Enemy("EnemyRobot", EnemyManager::EnemyType::Robot,
        Collision::Type::Enemy, this, 100.0f)
{
}

// 初期化
void EnemyRobot::Initialize()
{
    GetTransform()->SetSize(size_);
    GetTransform()->SetTexSize(size_);
    GetTransform()->SetPivot(size_ * 0.5f);
}

// 更新
void EnemyRobot::Update(const float& elapsedTime)
{
    const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();
    const DirectX::XMFLOAT2 enemyCenterPosition = GetTransform()->GetCenterPosition();
    moveDirection_ = XMFloat2Normalize(playerCenterPosition - enemyCenterPosition);


    // 追跡処理
    Pursuit(elapsedTime);

    // 今はボタンで判定してる(今後は敵の弾の処理を複数作って攻撃のステートに切り替える)
    if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_LEFT_SHOULDER)
    {
        // 弾の生成
        BulletGeneration(enemyCenterPosition);
    }
}

void EnemyRobot::DrawDebug()
{
#if USE_IMGUI
    ImGui::DragInt("Power", &power_);
#endif // USE_IMGUI
}

void EnemyRobot::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
}

// 追跡処理
void EnemyRobot::Pursuit(const float& elapsedTime)
{
    GetTransform()->AddPosition(moveDirection_ * moveSpeed_ * elapsedTime);
    GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(moveDirection_.y, moveDirection_.x) + DirectX::XM_PIDIV2));
}

// 弾の生成
void EnemyRobot::BulletGeneration(const DirectX::XMFLOAT2 enemyCenterPosition)
{
    const int bulletNum = 5;

    // 生成位置を決定する(4箇所)
    static const int generateNum = 5;
    constexpr float angle[generateNum] =
    {
        DirectX::XMConvertToRadians(50.0f), DirectX::XMConvertToRadians(-50.0f),
        DirectX::XMConvertToRadians(25.0f), DirectX::XMConvertToRadians(-25.0f),
        DirectX::XMConvertToRadians(0),
    };

    for (float generateIndex : angle)
    {
        // BulletStraight* bulletStraight = new BulletStraight;

        // 今は仮でホーミングとストレート扱ってる。もっときれいにしなあかん。
        BulletHoming* bulletStraight = new BulletHoming;

        const DirectX::XMFLOAT2 newMoveDirection =
        {
            moveDirection_.x * cosf(generateIndex) - moveDirection_.y * sinf(generateIndex),
            moveDirection_.x * sinf(generateIndex) + moveDirection_.y * cosf(generateIndex)
        };

        // 弾の生成位置を決める
        const DirectX::XMFLOAT2 generatePosition = enemyCenterPosition + newMoveDirection * generateOffset_;

        bulletStraight->Launch(generatePosition, newMoveDirection, power_);
    }
}
