#include "Enemy.h"
#include "Collision/CollisionManager.h"

Enemy::Enemy(const std::string& name, const EnemyManager::EnemyType& enemyType, const Collision::Type& type, Object* owner, const float& radius)
    : Character(name), enemyType_(enemyType), collision_(new Collision(type, owner, radius))
{
    // 自分自身を登録する
    EnemyManager::Instance().Register(this);

    stateMachine_.reset(new StateMachine<State<Enemy>>);
}

Enemy::~Enemy()
{
    CollisionRemove();
}

// 初期化
void Enemy::Initialize()
{
}

// 更新
void Enemy::Update(const float& elapsedTime)
{
    // 体力がなくなったら消滅する
    if (health_ <= 0)
    {
        EnemyManager::Instance().Remove(this);
    }
}

// コリジョンの後処理
void Enemy::CollisionRemove()
{
    CollisionManager::Instance().Remove(collision_);
    collision_ = nullptr;
}

void Enemy::ChangeState(const int& state)
{
    // 前回のステートを記録
    oldState_ = currentState_;

    // 現在のステートを記録
    currentState_ = state;

    stateMachine_.get()->ChangeState(state);
}