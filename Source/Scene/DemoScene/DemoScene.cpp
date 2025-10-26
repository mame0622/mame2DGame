#include "DemoScene.h"
#include "Graphics/Graphics.h"
#include "ImGui/ImGuiCtrl.h"
#include "Object/Character/Player/PlayerManager.h"
#include "Object/Character/Enemy/EnemyManager.h"
#include "Object/Bullet/BulletManager.h"
#include "Collision/CollisionManager.h"
#include "Object/Skill/SkillManager.h"
#include "Object/Character/Enemy/EnemyPlanaria/EnemyPlanaria.h"
#include "Object/Effect/EffectManager.h"

// リソース生成
void DemoScene::CreateResource()
{
    // Player生成
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();
}

// 初期化
void DemoScene::Initialize()
{
    PlayerManager::Instance().Initialize();
}

// 終了化
void DemoScene::Finalize()
{
    EnemyManager::Instance().Finalize();
}

// 更新
void DemoScene::Update(const float& elapsedTime)
{
    PlayerManager::Instance().Update(elapsedTime);

    SkillManager::Instance().Update(elapsedTime);

    EnemyManager::Instance().Update(elapsedTime);

    BulletManager::Instance().Update(elapsedTime);

    CollisionManager::Instance().Update();

    // 敵を定期的に生成する
    enemySpawnTimer_ -= elapsedTime;
    if (enemySpawnTimer_ <= 0.0f)
    {
        EnemyPlanaria* enemy0 = new EnemyPlanaria();
        EnemyPlanaria* enemy1 = new EnemyPlanaria();
        EnemyPlanaria* enemy2 = new EnemyPlanaria();
        EnemyPlanaria* enemy3 = new EnemyPlanaria();
        enemy0->GetTransform()->SetPosition({});
        enemy1->GetTransform()->SetPosition(SCREEN_WIDTH, 0.0f);
        enemy2->GetTransform()->SetPosition(0.0f, SCREEN_HEIGHT);
        enemy3->GetTransform()->SetPosition(SCREEN_WIDTH, SCREEN_HEIGHT);

        enemySpawnTimer_ = 3.0f;
    }

    EffectManager::Instance().Update(elapsedTime);
}

// 描画
void DemoScene::Render()
{
    Graphics::Instance().SetBlendState(Shader::BlendState::Alpha);
    Graphics::Instance().SetRasterizerState(Shader::RasterState::CullNone);
    Graphics::Instance().SetDepthStencileState(Shader::DepthState::ZT_OFF_ZW_OFF);

    EffectManager::Instance().Render();
    
    PlayerManager::Instance().Render();

    EnemyManager::Instance().Render();

    BulletManager::Instance().Render();

    SkillManager::Instance().Render();

}

// ImGui
void DemoScene::DrawDebug()
{
    PlayerManager::Instance().DrawDebug();

    EnemyManager::Instance().DrawDebug();

    BulletManager::Instance().DrawDebug();

    SkillManager::Instance().DrawDebug();

    EffectManager::Instance().DrawDebug();
}
