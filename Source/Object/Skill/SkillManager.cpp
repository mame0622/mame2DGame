#include "SkillManager.h"
#include "Application/Common.h"
#include "Skill.h"
#include "ImGui/ImGuiCtrl.h"

SkillManager::SkillManager()
{
    // ƒeƒNƒXƒ`ƒƒ“o˜^
    spriteBatches_.emplace_back(SpriteBatch(L"./Resources/Image/Skill/ChainLightning/ChainLightning.png", 50/*Å‘å•`‰æ”*/));
    spriteBatches_.emplace_back(SpriteBatch(L"./Resources/Image/Skill/SplitThunder/SplitThunder.png", 1000/*Å‘å•`‰æ”*/));
}

// XV
void SkillManager::Update(const float& elapsedTime)
{
    // “o˜^
    for (Skill* skill : generates_)
    {
        skills_.emplace_back(skill);
    }
    generates_.clear();

    // XV
    for (Skill*& skill : skills_)
    {
        skill->Update(elapsedTime);
    }

    // ”jŠü
    for (Skill* skill : removes_)
    {
        auto it = std::find(skills_.begin(), skills_.end(), skill);

        if (it != skills_.end()) skills_.erase(it);

        SafeDeletePtr(skill);
    }
    removes_.clear();
}

void SkillManager::Render()
{
    // ƒeƒNƒXƒ`ƒƒ‚ÌŽí—Þ‚²‚Æ‚É•`‰æ‚·‚é
    for (int textureIndex = 0; textureIndex < static_cast<int>(SkillType::Max); ++textureIndex)
    {
        std::vector<Transform2D> transforms;
        for (Skill*& skill : skills_)
        {
            if (skill->GetSkillType() == static_cast<SkillType>(textureIndex))
            {
                if (skill->GetTransformCount() == 0) continue;

                for (int i = 0; i < skill->GetTransformCount(); ++i)
                {
                    transforms.emplace_back(skill->GetTransform(i));
                }
            }
        }
        spriteBatches_.at(textureIndex).Render(transforms);
    }
}

void SkillManager::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin("SkillManager");

    for (Skill*& skill : skills_)
    {
        skill->DrawDebug();
    }

    ImGui::End();

#endif // USE_IMGUI
}