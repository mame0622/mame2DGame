#include "EnemyCharger.h"
#include "Object/Character/Player/PlayerManager.h"
#include "ImGui/ImGuiCtrl.h"

EnemyCharger::EnemyCharger()
    :Enemy("EnemyCharger", EnemyManager::EnemyType::Charger,
        Collision::Type::Enemy, this, 25.0f)
{
}

// 初期化
void EnemyCharger::Initialize()
{
    const float size = 50.0f;
    GetTransform()->SetSize(size);
    GetTransform()->SetTexSize(size);
    GetTransform()->SetPivot(size * 0.5f);
}

// 更新処理
void EnemyCharger::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    // 進行方向ベクトル算出
    const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();
    const DirectX::XMFLOAT2 enemyCenterPosition = GetTransform()->GetCenterPosition();
    moveVec_ = playerCenterPosition - enemyCenterPosition;

    switch (state_)
    {
    case State::Pursuit:
        // 追跡処理
        Pursuit(elapsedTime);
        break;
    case State::ChargeAttack:
        // 突進攻撃
        ChargeAttack(elapsedTime);
        break;
    }
}
// ImGui
void EnemyCharger::DrawDebug()
{
#if USE_IMGUI
    const std::string name = GetName() + std::to_string(GetObjectId());
    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed))
    {
        Object::DrawDebug();
        Character::DrawDebug();

        ImGui::TreePop();
    }
#endif // USE_IMGUI
}

// 衝突検知
void EnemyCharger::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
}

// 追跡処理
void EnemyCharger::Pursuit(const float& elapsedTime)
{
    moveSpeed_ = 200.0f;
    // 旋回処理
    Turn();

    const float length = XMFloat2Length(moveVec_);

    // 突進攻撃に切り替わる範囲
    const float chargeRange = 300.0f;

    //ステート更新
    if (length < chargeRange)
    {
        moveSpeed_ = 0;
        chargeTime_ = 0;
        state_ = State::ChargeAttack;
    }

    // 移動処理
    Move(elapsedTime);
}

// 突進処理
void EnemyCharger::ChargeAttack(const float& elapsedTime)
{
    // 突進準備開始
    chargeTime_ += elapsedTime;
    if (chargeTime_ < 2.0f)
    {
        // 旋回処理
        Turn();

        // 突進スピード設定
        moveSpeed_ = 3000.0f;
    }
    else
    {
        // 減速
        const float deceleration = 20.0f;
        moveSpeed_ -= deceleration;
        
        // 前方向ベクトル算出
        const float angleRadians = DirectX::XMConvertToRadians(GetTransform()->GetAngle());
        const DirectX::XMFLOAT2 forward = { sinf(angleRadians),-cosf(angleRadians) };
        GetTransform()->AddPosition(forward * moveSpeed_ * elapsedTime);

        // ステート更新
        if (moveSpeed_ <= 0)
        {
            moveSpeed_ = 0;
            state_ = State::Pursuit;
        }
    }
}

// 旋回処理
void EnemyCharger::Turn()
{
    const DirectX::XMFLOAT2 moveDirection = XMFloat2Normalize(moveVec_);

    GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(moveDirection.y, moveDirection.x) + DirectX::XM_PIDIV2));
}

// 移動処理
void EnemyCharger::Move(const float& elapsedTime)
{
    const DirectX::XMFLOAT2 moveDirection = XMFloat2Normalize(moveVec_);

    GetTransform()->AddPosition(moveDirection * moveSpeed_ * elapsedTime);
}


