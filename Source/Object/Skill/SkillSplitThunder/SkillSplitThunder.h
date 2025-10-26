#pragma once
#include "Object/Skill/Skill.h"
#include "Collision/Collision.h"

class SkillSplitThunder : public Skill
{
public:
    SkillSplitThunder();
    ~SkillSplitThunder() override {}

    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

    void Launch(const int& splitCount, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& oldDirection, const float& angle);
    void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) override;

private:
    void CollisionRemove();

private:
    Collision* collision_;

    // ---------- Move ----------
    DirectX::XMFLOAT2   moveDirection_  = {};
    float               moveSpeed_      = 700.0f;

    const int   maxSplint_  = 3;
    int         splitCount_ = 0;
};

