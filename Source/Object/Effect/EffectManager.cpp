#include "EffectManager.h"
#include "Effect.h"
#include "Application/Common.h"
#include "ImGui/ImGuiCtrl.h"

#include "EffectBlink/EffectBlink.h"

EffectManager::EffectManager()
{
    spriteBatches_.push_back(SpriteBatch(L"./Resources/Image/Player/Blink.png", 50/*最大描画数*/));
}

void EffectManager::Update(const float& elapsedTime)
{
    // 生成処理
    for (Effect* effect : generates_)
    {
        effects_.emplace_back(effect);
    }
    generates_.clear();

    // 更新
    for (Effect*& effect : effects_)
    {
        effect->Update(elapsedTime);
    }

    // 削除処理
    for (Effect* effect : removes_)
    {
        auto it = std::find(effects_.begin(), effects_.end(), effect);

        if (it != effects_.end()) effects_.erase(it);

        SafeDeletePtr(effect);
    }
    removes_.clear();
}

void EffectManager::Render()
{
    // テクスチャの種類ごとに描画する
    for (int i = 0; i < static_cast<int>(EffectType::Max); ++i)
    {
        std::vector<Transform2D> transforms;
        for (Effect*& effect : effects_)
        {
            if (effect->GetEffectType() == static_cast<EffectType>(i))
            {
                transforms.emplace_back(*effect->GetTransform());
            }
        }

        spriteBatches_.at(i).Render(transforms);
    }
}

void EffectManager::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin("EffectManager");

    if (ImGui::Button("EffectBlink"))
    {
        EffectBlink* effect = new EffectBlink();
    }

    ImGui::End();
#endif // USE_IMGUI
}

void EffectManager::Clear()
{
    for (Effect*& effect : effects_)
    {
        SafeDeletePtr(effect);
    }
    effects_.clear();
    effects_.shrink_to_fit();
}