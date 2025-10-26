#include "BulletManager.h"
#include "Object/Character/Player/PlayerManager.h"
#include "ImGui/ImGuiCtrl.h"
#include "Application/Common.h"
#include "Object/Bullet/BulletLevel1/BulletLevel1.h"

BulletManager::BulletManager()
{
    // 画像登録
    spriteBatches_.emplace_back(L"./Resources/Image/Bullet/Bullet_Level_1.png", 50/*最大描画数*/);
    spriteBatches_.emplace_back(L"./Resources/Image/Bullet/Bullet_Level_2.png", 50);
    spriteBatches_.emplace_back(L"./Resources/Image/Bullet/Bullet_Level_3.png", 50);
    spriteBatches_.emplace_back(L"./Resources/Image/Bullet/Bullet_Level_4.png", 50);
    spriteBatches_.emplace_back(L"./Resources/Image/Bullet/Bullet_Level_5.png", 50);
}

// 初期化
void BulletManager::Initialize()
{
    // プレイヤーの周りを回るBullet生成
    for (int orbitBullet = 0; orbitBullet < maxOrvitBullets_; ++orbitBullet)
    {
        BulletOrbit* bulletOrbit = new BulletOrbit();
        bulletOrbit->Initialize(bulletSize_);
        orvitBullets_.emplace_back(bulletOrbit);
    }
}

// 更新
void BulletManager::Update(const float& elapsedTime)
{
    // 生成処理
    for (Bullet* firedBullet : generates_)
    {
        firedBullets_.emplace_back(firedBullet);
    }
    generates_.clear();

    // 更新
    for (Bullet*& firedBullet : firedBullets_)
    {
        firedBullet->Update(elapsedTime);
    }

    // 削除処理
    for (Bullet* firedBullet : removes_)
    {
        auto it = std::find(firedBullets_.begin(), firedBullets_.end(), firedBullet);

        if (it != firedBullets_.end()) firedBullets_.erase(it);

        SafeDeletePtr(firedBullet);
    }
    removes_.clear();

    // Playerの周りに配置するBullet更新
    UpdateOrvitBullets(elapsedTime);
}

// 描画
void BulletManager::Render()
{
    const int maxBulletLevel = 5;
    for (int i = 0; i < maxBulletLevel; ++i)
    {
        std::vector<Transform2D> transforms_;
        for (BulletOrbit*& orvitBullet : orvitBullets_)
        {
            if (orvitBullet->GetBulletLevel() == static_cast<Bullet::BulletLevel>(i))
            {                
                transforms_.emplace_back(*orvitBullet->GetTransform());
            }
        }

        spriteBatches_.at(i).Render(transforms_);
    }

    // 一旦ここで書くけど、上とまとめれたらいいな～～～
    for (int i = 0; i < maxBulletLevel; ++i)
    {
        std::vector<Transform2D> transforms_;
        for (Bullet*& firedBullet : firedBullets_)
        {
            if (firedBullet->GetBulletLevel() == static_cast<Bullet::BulletLevel>(i))
            {
                transforms_.emplace_back(*firedBullet->GetTransform());
            }
        }

        spriteBatches_.at(i).Render(transforms_);
    }
}

// ImGui
void BulletManager::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin("BulletManager");
    
    ImGui::DragFloat("Radius", &orvitRadius_, 1.0f);

    ImGui::DragFloat("Angle", &orvitAngle_, 1.0f);

    ImGui::End(); // BulletManager

    ImGui::Begin("FiredBullets List");

    for (Bullet*& firedBullet : firedBullets_)
    {
        firedBullet->DrawDebug();
    }

    ImGui::End(); // FiredBullets List

    ImGui::Begin("OrvitBullet List");

    for (BulletOrbit*& bullet : orvitBullets_)
    {
        bullet->DrawDebug();
    }

    ImGui::End(); // OrvitBullet List

#endif // USE_IMGUI
}

// 発射
void BulletManager::Launch()
{
    // 弾のアクティブリストを作成
    const bool activeBulletList[maxOrvitBullets_] =
    {
        orvitBullets_.at(0)->IsCollisionActive(),
        orvitBullets_.at(1)->IsCollisionActive(),
        orvitBullets_.at(2)->IsCollisionActive(),
        orvitBullets_.at(3)->IsCollisionActive(),
        orvitBullets_.at(4)->IsCollisionActive(),
    };

    // 発射できる弾があるか確認
    bool isReadyToFire = false;
    for (int bulletIndex = 0; bulletIndex < maxOrvitBullets_; ++bulletIndex)
    {
        if (activeBulletList[bulletIndex]) isReadyToFire = true;
    }
    if (isReadyToFire == false) return;

    // ---------- Index順に発射 ----------
    // 発射番号を決定する
    while (true)
    {
        // 発射可能個体を探す
        if (activeBulletList[launchIndex_] == false)
        {
            // リスト番号を次に進める
            ++launchIndex_;
            if (launchIndex_ >= maxOrvitBullets_) launchIndex_ = 0;
        }
        else
        {
            // 発射可能個体 発見
            break;
        }
    }

    const Bullet::BulletLevel level = orvitBullets_.at(launchIndex_)->GetBulletLevel();
    const DirectX::XMFLOAT2 direction = PlayerManager::Instance().GetPlayer()->GetMoveDirection();
    
    switch (level)
    {
    case Bullet::BulletLevel::One:
    {
        BulletLevel1* level1 = new BulletLevel1();
        level1->Launch(launchIndex_, direction, orvitRadius_);

        orvitBullets_.at(launchIndex_)->SetCollisionActive(false);
    }
        break;
    case Bullet::BulletLevel::Two:
        break;
    case Bullet::BulletLevel::Three:
        break;
    case Bullet::BulletLevel::Four:
        break;
    case Bullet::BulletLevel::Five:
        break;
    }

    ++launchIndex_;
    if (launchIndex_ >= maxOrvitBullets_) launchIndex_ = 0;
}

// 全削除
void BulletManager::Clear()
{
    for (Bullet*& firedBullet : firedBullets_)
    {
        SafeDeletePtr(firedBullet);
    }
    firedBullets_.clear();
    firedBullets_.shrink_to_fit();
}

// Playerの周りに配置するBullet更新
void BulletManager::UpdateOrvitBullets(const float& elapsedTime)
{
    for (BulletOrbit*& orvitBullet : orvitBullets_)
    {
        orvitBullet->Update(elapsedTime);
    }

    // Playerの画像の中心をTargetに設定する
    const DirectX::XMFLOAT2 playerCenterPosition = PlayerManager::Instance().GetTransform()->GetCenterPosition();

    // Targetに対して円状になるようにBulletを配置する
    for (int i = 0; i < maxOrvitBullets_; ++i)
    {
        float angle = orvitAngle_ + (2.0f * DirectX::XM_PI / maxOrvitBullets_) * i;

        // Bulletの画像の中心を指定された位置に持っていく為のOffset
        const DirectX::XMFLOAT2 orvitBulletOffset = orvitBullets_.at(i)->GetTransform()->GetSize() * 0.5f;

        orvitBullets_.at(i)->GetTransform()->SetPositionX(playerCenterPosition.x + orvitRadius_ * cosf(angle) - orvitBulletOffset.x);
        orvitBullets_.at(i)->GetTransform()->SetPositionY(playerCenterPosition.y + orvitRadius_ * sinf(angle) - orvitBulletOffset.y);
    }

    // Targetに対して辺が向くようにする
    for (int i = 0; i < maxOrvitBullets_; ++i)
    {
        const DirectX::XMFLOAT2 orvitBulletCenterPosition = orvitBullets_.at(i)->GetTransform()->GetCenterPosition();

        const DirectX::XMFLOAT2 direction = XMFloat2Normalize(orvitBulletCenterPosition - playerCenterPosition);

        orvitBullets_.at(i)->GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(direction.y, direction.x) + DirectX::XM_PIDIV2));
    }

    orvitAngle_ += elapsedTime;
}
