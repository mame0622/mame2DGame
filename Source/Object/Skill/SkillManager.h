#pragma once
#include <vector>
#include <set>
#include "Resource/Sprite/SpriteBatch.h"

class Skill;

class SkillManager
{
public:
    enum class SkillType
    {
        ChainLightning,
        SkillSplitThunder,
        Max,
    };

private:
    SkillManager();
    ~SkillManager() {}

public:
    static SkillManager& Instance()
    {
        static SkillManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

    void Register(Skill* skill) { generates_.insert(skill); }
    void Remove(Skill* skill) { removes_.insert(skill); }
    void Clear();

private:
    std::vector<Skill*>         skills_;
    std::set<Skill*>            generates_;
    std::set<Skill*>            removes_;
    std::vector<SpriteBatch>    spriteBatches_;
};

