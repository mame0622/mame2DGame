#pragma once
#include "Object/Character/Enemy/Enemy.h"

class EnemyCharger : public Enemy
{
public:
    EnemyCharger();
    ~EnemyCharger() {}

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

private:
    void Pursuit(const float& elapsedTime);
    void ChargeAttack(const float& elapsedTime);
    void Turn();
    void Move(const float& elapsedTime);


private:
    DirectX::XMFLOAT2 moveVec_ = {};

    float moveSpeed_ = 200.0f;

    float chargeTime_ = 0;

    enum class State
    {
        Pursuit,
        ChargeAttack,
        End
    };

    State state_ = State::Pursuit;
};