#pragma once
#include "Object/Character/Enemy/Enemy.h"

class EnemyNecromancer : public Enemy
{
public:
    enum class State
    {
        Pursuit,
        Necromancy,
        End
    };

public:
    EnemyNecromancer();
    ~EnemyNecromancer(){}

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

    // ---------- StateMachine ----------
    void ChangeState(const State& state) { Enemy::ChangeState(static_cast<int>(state)); }

private:
    void Turn();

private:
    const float size_ = 100.0f;
};
