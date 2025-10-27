#include "EnemyMadScientistState.h"
#include "EnemyMadScientist.h"
#include "Object/Character/Player/PlayerManager.h"
#include "Object/Character/Enemy/EnemyRobot/EnemyRobot.h"

// DecisionState
namespace EnemyMadScientistState
{
    // 初期化
    void DecisionState::Initialize()
    {
    }

    // 更新
    void DecisionState::Update(const float& elapsedTime)
    {
    }

    // ImGui
    void DecisionState::DrawDebug()
    {
    }
}

// SummonState
namespace EnemyMadScientistState
{
    // 初期化
    void SummonState::Initialize()
    {
        // EnemyRobotを召喚する
        const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();
        const DirectX::XMFLOAT2 ownerCenterPosition = owner_->GetTransform()->GetCenterPosition();
        const DirectX::XMFLOAT2 toPlayerDirection = XMFloat2Normalize(playerCenterPosition - ownerCenterPosition);

        EnemyRobot* robot = new EnemyRobot;
        // EnemyManagerでもInitialize呼び出されるが、すぐにSizeを使用するために呼び出す
        robot->Initialize(); 
        const DirectX::XMFLOAT2 ownerPosition = owner_->GetTransform()->GetPosition();
        const DirectX::XMFLOAT2 robotSize = robot->GetTransform()->GetSize();
        robot->GetTransform()->SetPosition(ownerPosition + toPlayerDirection * length_ - robotSize * 0.5f);

        recoveryTimer_ = recoveryTime_;
    }

    // 更新
    void SummonState::Update(const float& elapsedTime)
    {
        recoveryTimer_ -= elapsedTime;

        if (recoveryTimer_ <= 0.0f)
        {
            EnemyMadScientist* owner = dynamic_cast<EnemyMadScientist*>(owner_);
            owner->ChangeState(EnemyMadScientist::State::Decision);
            return;
        }
    }

    // ImGui
    void SummonState::DrawDebug()
    {
    }
}

// HideState
namespace EnemyMadScientistState
{
    // 初期化
    void HideState::Initialize()
    {
    }

    // 更新
    void HideState::Update(const float& elapsedTime)
    {
        // hidePositionを求める
        const bool changeState = CalcHidePosition();

        // Robotが存在しないのでステート変更
        if (changeState == false)
        {
            EnemyMadScientist* owner = dynamic_cast<EnemyMadScientist*>(owner_);
            owner->ChangeState(EnemyMadScientist::State::Decision);
            return;
        }

        // 移動処理
        const DirectX::XMFLOAT2 ownerCenterPosition = owner_->GetTransform()->GetCenterPosition();
        const DirectX::XMFLOAT2 moveDirection = XMFloat2Normalize(hidePosition_ - ownerCenterPosition);
        owner_->GetTransform()->AddPosition(moveDirection * owner_->GetMoveSpeed() * elapsedTime);
    }

    // ImGui
    void HideState::DrawDebug()
    {
    }

    // hidePositionを求める Robotが存在しない場合False
    const bool HideState::CalcHidePosition()
    {
        bool isEnemyRobotAlive = false;

        // 自分から最も近いRobotの位置を記憶する
        const DirectX::XMFLOAT2 ownerCenterPosition = owner_->GetTransform()->GetCenterPosition();
        DirectX::XMFLOAT2 mostNearRobotPosition = {};
        float mostNearLength = FLT_MAX;
        for (Enemy*& enemy : EnemyManager::Instance().GetEnemies())
        {
            if (enemy->GetEnemyType() != EnemyManager::EnemyType::Robot) continue;

            // Robotが1体以上存在する
            isEnemyRobotAlive = true;

            const DirectX::XMFLOAT2 robotCenterPosition = enemy->GetTransform()->GetCenterPosition();
            const float length = XMFloat2Length(ownerCenterPosition - robotCenterPosition);

            if (mostNearLength >= length)
            {
                mostNearLength = length;
                mostNearRobotPosition = robotCenterPosition;
            }
        }

        if (isEnemyRobotAlive == false) return false;

        // Playerから見てRobotの後ろになる位置をhidePositionとする
        const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();
        const DirectX::XMFLOAT2 toPlayerDirection = XMFloat2Normalize(mostNearRobotPosition - playerCenterPosition);
        const DirectX::XMFLOAT2 offset = owner_->GetTransform()->GetSize() * 0.5f;
        hidePosition_ = mostNearRobotPosition + toPlayerDirection * hideLength_ - offset;

        return true;
    }
}

// RetreatState
namespace EnemyMadScientistState
{
    // 初期化
    void RetreatState::Initialize()
    {
    }

    // 更新
    void RetreatState::Update(const float& elapsedTime)
    {
    }

    // ImGui
    void RetreatState::DrawDebug()
    {
    }
}