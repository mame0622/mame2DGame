#pragma once
#include "Object/Skill/Skill.h"

class SkillChainLightning : public Skill
{
public:
    SkillChainLightning();
    ~SkillChainLightning() override {}

    const bool Initialize();
    void Update(const float& elapsedTime) override;
    void DrawDebug() override;

private:
    void FindNearestEnemy(const int& index);

    void BuildTransform(const DirectX::XMFLOAT2& positionA, const DirectX::XMFLOAT2& positionB,
        const DirectX::XMFLOAT2& sizeA, const int& transformIndex);

private:
    std::vector<int> enemyIndexes_;

    float delayTimer_   = 0.5f;
    float destroyTimer_ = 1.0f;
    float destroySpeed_ = 2.0f;
};

