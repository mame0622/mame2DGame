#pragma once
#include "Resource/Sprite/SpriteBatch.h"
#include <vector>
#include <set>

class Effect;

class EffectManager
{
public:
    enum class EffectType
    {
        Blink,
        Max,
    };

private:
    EffectManager();
    ~EffectManager() {}

public:
    static EffectManager& Instance()
    {
        static EffectManager instance;
        return instance;
    }

    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

    void Register(Effect* effect) { generates_.insert(effect); }
    void Remove(Effect* effect) { removes_.insert(effect); }
    void Clear();

private:
    std::vector<SpriteBatch> spriteBatches_;
    std::vector<Effect*>    effects_;
    std::set<Effect*>       generates_;
    std::set<Effect*>       removes_;
};

