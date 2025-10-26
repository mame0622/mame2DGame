#include "Player.h"
#include "ImGui/ImGuiCtrl.h"
#include "Input/Input.h"
#include "Object/Bullet/BulletManager.h"
#include "Graphics/Graphics.h"
#include "Object/Skill/SkillChainLightning/SkillChainLightning.h"
#include "Object/Skill/SkillSplitThunder/SkillSplitThunder.h"

#include "Object/Effect/EffectBlink/EffectBlink.h"

Player::Player()
    : Character("Player"), 
    spriteBatch_(L"./Resources/Image/Player/Player.png", 1),
    collision_(new Collision(Collision::Type::Player, this, 25.0f))
{
}

// 初期化
void Player::Initialize()
{
    GetTransform()->SetPosition(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

    GetTransform()->SetSize(50.0f);
    GetTransform()->SetTexSize(50.0f);
    GetTransform()->SetPivot(25.0f);
}

// 更新
void Player::Update(const float& elapsedTime)
{
    // 移動処理
    Move(elapsedTime);

    // 旋回処理
    Turn();

    // 回避 Shift
    if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER)
    {
        Blink();
    }

    // 弾発射 Aボタン Space
    if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_A)
    {
        BulletManager::Instance().Launch();
    }

    // スキル発動
    if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_X)
    {
        SkillChainLightning* skill = new SkillChainLightning();
        if (skill->Initialize() == false)
        {
            SkillManager::Instance().Remove(skill);
        }
    }

    if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_Y)
    {
        const DirectX::XMFLOAT2 generatePosition = GetTransform()->GetPosition();

        SkillSplitThunder* skill0 = new SkillSplitThunder();
        SkillSplitThunder* skill1 = new SkillSplitThunder();
        SkillSplitThunder* skill2 = new SkillSplitThunder();
        skill0->Launch(0, generatePosition, moveDirection_, 0.0f);
        skill1->Launch(0, generatePosition, moveDirection_, DirectX::XMConvertToRadians(120));
        skill2->Launch(0, generatePosition, moveDirection_, DirectX::XMConvertToRadians(240));
    }

    // 画面内に位置を収める
    ClampPosition();

    ++currentEffectFrameCount_;
    if (currentEffectFrameCount_ == maxEffectFrameCount_[currentEffectCount_])
    {
        if (currentEffectCount_ < maxEffectCount_)
        {
            EffectBlink* effect = new EffectBlink();
            effect->Initialize(GetTransform()->GetCenterPosition(),
                effectSize[currentEffectCount_].x, effectSize[currentEffectCount_].y);

            currentEffectFrameCount_ = 0;
            ++currentEffectCount_;
        }
    }
}

// 描画
void Player::Render()
{
    std::vector<Transform2D> transforms_;
    transforms_.emplace_back(*GetTransform());
    spriteBatch_.Render(transforms_);
}

// ImGui
void Player::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName().c_str());

    ImGui::DragFloat2("EffectSize0", &effectSize[0].x);
    ImGui::DragFloat2("EffectSize1", &effectSize[1].x);
    ImGui::DragFloat2("EffectSize2", &effectSize[2].x);
    ImGui::DragInt("FrameCount0", &maxEffectFrameCount_[0]);
    ImGui::DragInt("FrameCount1", &maxEffectFrameCount_[1]);
    ImGui::DragInt("FrameCount2", &maxEffectFrameCount_[2]);

    ImGui::DragFloat("MoveSpeed", &moveSpeed_);
    ImGui::DragFloat("BlinkPower", &blinkPower_);
    ImGui::DragFloat("Deceleration", &deceleration_);

    ImGui::DragFloat2("CollisionOffset", &collisionOffset_.x);

    Object::DrawDebug();
    Character::DrawDebug();

    ImGui::End();
#endif
}

// 衝突検知
void Player::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{
    if (type == Collision::Type::Enemy)
    {

    }
}

// 移動処理
void Player::Move(const float& elapsedTime)
{
    // 入力による移動
    const float aLx = Input::Instance().GetGamePad().GetAxisLx();
    const float aLy = Input::Instance().GetGamePad().GetAxisLy();
    const DirectX::XMFLOAT2 moveDirection = XMFloat2Normalize({ aLx, -aLy });
    if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
    {
        moveDirection_ = moveDirection;
        blinkDirection_ = moveDirection;
    }    
    GetTransform()->AddPosition(moveDirection * moveSpeed_ * elapsedTime);

    // 回避による移動
    currentBlinkPower_ -= deceleration_ * elapsedTime;
    if (currentBlinkPower_ > 0.0f)
    {
        GetTransform()->AddPosition(blinkDirection_ * currentBlinkPower_ * elapsedTime);
    }
}

// 旋回処理
void Player::Turn()
{
    // 入力がなければ旋回処理を行わない
    if (moveDirection_.x == 0.0f && moveDirection_.y == 0.0f) return;

    GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(moveDirection_.y, moveDirection_.x) + DirectX::XM_PIDIV2));
}

// 回避
void Player::Blink()
{
    blinkDirection_ = moveDirection_;
    currentBlinkPower_ = blinkPower_;

    currentEffectCount_ = 0;
    currentEffectFrameCount_ = 0;
}

// 画面内に位置を収める
void Player::ClampPosition()
{
    DirectX::XMFLOAT2 position = GetTransform()->GetPosition();
    const DirectX::XMFLOAT2 size = GetTransform()->GetSize();
    const DirectX::XMFLOAT2 collisionWidth = { position.x - collisionOffset_.x, position.x + size.x + collisionOffset_.x };
    const DirectX::XMFLOAT2 collisionHeight = { position.y - collisionOffset_.y, position.y + size.y + collisionOffset_.y };
    if (collisionWidth.x <= 0.0f) position.x = collisionOffset_.x;
    if (collisionWidth.y >= SCREEN_WIDTH) position.x = SCREEN_WIDTH - size.x - collisionOffset_.x;
    if (collisionHeight.x <= 0.0f) position.y = collisionOffset_.y;
    if (collisionHeight.y >= SCREEN_HEIGHT) position.y = SCREEN_HEIGHT - size.y - collisionOffset_.y;
    GetTransform()->SetPosition(position);
}
