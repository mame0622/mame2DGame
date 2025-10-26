#include "Effect.h"

Effect::Effect(const std::string& name, const EffectManager::EffectType& effectType)
    : Object(name), effectType_(effectType)
{
    // 自身をマネージャーに登録
    EffectManager::Instance().Register(this);
}