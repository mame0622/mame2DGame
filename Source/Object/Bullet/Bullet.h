#pragma once
#include "Object/Object.h"
#include "Collision/Collision.h"

class Bullet : public Object
{
public:
    enum class BulletLevel { One, Two, Three, Four, Five };

public:
    Bullet(const std::string& name, const BulletLevel& level,
        const Collision::Type& type, Object* owner, const float& radius);
    ~Bullet() override {}

    virtual void Update(const float& elapsedTime);
    void DrawDebug() override;

    const BulletLevel GetBulletLevel() const { return level_; }
    void SetLevel(const BulletLevel& level) { level_ = level; }

    void SetBulletNumber(const int& bulletNumber) { bulletNumber_ = bulletNumber; }
    const int GetBulletNumber() const { return bulletNumber_; }

    // CollisionÇÃå„èàóù
    void CollisionRemove();

private:
    BulletLevel level_ = BulletLevel::One;
    
    // ---------- égópÇµÇƒÇ¢ÇÈBullet Number ----------
    int bulletNumber_ = 0;

    Collision* collision_;
};

