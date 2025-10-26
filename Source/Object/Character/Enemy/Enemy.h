#pragma once
#include "Object/Character/Character.h"
#include "EnemyManager.h"
#include "Collision/Collision.h"
#include "Ai/StateMachine/StateMachine.h"
#include <memory>

class Enemy : public Character
{
public:
    Enemy(const std::string& name, const EnemyManager::EnemyType& enemyType,
        const Collision::Type& type, Object* owner, const float& radius);
    virtual ~Enemy();

    virtual void Initialize();
    virtual void Update(const float& elapsedTime);

    const EnemyManager::EnemyType GetEnemyType() const { return enemyType_; }
    
    // ---------- ‘Ì—Í ----------
    const int GetHealth() const { return health_; }
    void SetHealth(const int& health) { health_ = health; }
    void Damage(const int& damage) { health_ -= damage; }

    // ---------- StateMachine ----------
    StateMachine<State<Enemy>>* GetStateMachine() { return stateMachine_.get(); }
    void ChangeState(const int& state);
    const int GetCurrentState() const { return currentState_; }
    const int GetOldState() const { return oldState_; }
    void SetCurrentState(const int& state) { currentState_ = state; }

    // ---------- Move ----------
    const float GetMoveSpeed() const { return moveSpeed_; }
    void SetMoveSpeed(const float& speed) { moveSpeed_ = speed; }

private:
    // Collision‚ÌŒãˆ—
    void CollisionRemove();

private:
    const EnemyManager::EnemyType enemyType_ = EnemyManager::EnemyType::Normal;

    Collision* collision_;

    int health_ = 1;

    // ---------- StateMachine ----------
    std::unique_ptr<StateMachine<State<Enemy>>> stateMachine_;
    int currentState_   = 0;
    int oldState_       = 0;

    // ---------- Move ----------
    float moveSpeed_ = 0.0f;
};

