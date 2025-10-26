#pragma once
#include "Collision.h"
#include <DirectXMath.h>
#include <vector>
#include <set>

class CollisionManager
{
private:
    CollisionManager() {}
    ~CollisionManager() {}

public:
    static CollisionManager& Instance()
    {
        static CollisionManager instance;
        return instance;
    }

    void Update();

    // ‰~ Vs ‰~
    const bool IntersectCircleVsCircle(
        const DirectX::XMFLOAT2& positionA, const float& radiusA,
        const DirectX::XMFLOAT2& positionB, const float& radiusB,
        const DirectX::XMFLOAT2& sizeB, DirectX::XMFLOAT2& resultPosition);

    void Register(Collision* collision) { generates_.insert(collision); }
    void Remove(Collision* collision) { removes_.insert(collision); }

private:
    std::vector<Collision*> collisions_;
    std::set<Collision*>    generates_;
    std::set<Collision*>    removes_;

    const bool collisionMatrix[(int)Collision::Type::Max][(int)Collision::Type::Max] =
    {
        //
        /* Player      */ { false, true, false, false },
        /* Enemy       */ { true,  true, true, true },
        /* BulletOrbit */ { false, true, false, false },
        /* Bullet      */ { false, true, false, false },
    };
};

