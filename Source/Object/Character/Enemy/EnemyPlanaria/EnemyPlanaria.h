#pragma once
#include "Object/Character/Enemy/Enemy.h"

class EnemyPlanaria : public Enemy
{
public:
    EnemyPlanaria();
    ~EnemyPlanaria(){}

    void Initialize() override;
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

private:
    void Pursuit(const float& elapsedTime);

private:
    float moveSpeed_ = 0.0f;
};