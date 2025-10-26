#pragma once
#include <vector>
#include <set>
#include "Resource/Sprite/SpriteBatch.h"

class Enemy;

class EnemyManager
{
public:
    enum class EnemyType : int
    {
        Normal,
        Charger,
        Planaria,
        Necromancer,
        MadScientist,
        Robot,
        Max,
    };

private:
    EnemyManager();
    ~EnemyManager() {}

public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Finalize();
    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

    void Register(Enemy* enemy) { generates_.insert(enemy); }
    void Remove(Enemy* enemy) { removes_.insert(enemy); }
    void Clear();

    const int GetEnemyCount() const { return static_cast<int>(enemies_.size()); }
    Enemy* GetEnemy(const int& enemyIndex) { return enemies_.at(enemyIndex); }
    std::vector<Enemy*> GetEnemies() { return enemies_; }

private:
    std::vector<Enemy*>         enemies_;
    std::set<Enemy*>            generates_;
    std::set<Enemy*>            removes_;
    std::vector<SpriteBatch>    spriteBatches_;
};

