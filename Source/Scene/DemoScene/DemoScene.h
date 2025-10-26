#pragma once
#include "Scene/BaseScene.h"
#include <memory>

class DemoScene : public BaseScene
{
public:
    DemoScene() {}
    ~DemoScene() {}

    void CreateResource()                   override; // リソース生成
    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Update(const float& elapsedTime)   override; // 更新
    void Render()                           override; // 描画
    void DrawDebug()                        override; // ImGui

private:
    float enemySpawnTimer_ = 0.0f;
};

