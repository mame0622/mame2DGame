#pragma once
#include "Object/Object.h"
#include "EffectManager.h"

class Effect : public Object
{
public:
    Effect(const std::string& name, const EffectManager::EffectType& effectType);
    ~Effect() override {}

    virtual void Update(const float& elapsedTime) = 0;

    const EffectManager::EffectType GetEffectType() const { return effectType_; }

private:
    const EffectManager::EffectType effectType_;
};

