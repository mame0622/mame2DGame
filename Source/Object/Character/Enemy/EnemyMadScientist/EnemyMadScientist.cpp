#include "EnemyMadScientist.h"
#include "EnemyMadScientistState.h"

EnemyMadScientist::EnemyMadScientist()
    :Enemy("EnemyMadScientist", EnemyManager::EnemyType::MadScientist,
        Collision::Type::Enemy, this, 25.0f)
{
    GetStateMachine()->RegisterState(new EnemyMadScientistState::DecisionState(this));
    GetStateMachine()->RegisterState(new EnemyMadScientistState::SummonState(this));
    GetStateMachine()->RegisterState(new EnemyMadScientistState::HideState(this));
    GetStateMachine()->RegisterState(new EnemyMadScientistState::RetreatState(this));

    GetStateMachine()->SetState(static_cast<int>(State::Decision));
    SetCurrentState(static_cast<int>(State::Decision));
}

// 初期化
void EnemyMadScientist::Initialize()
{
    GetTransform()->SetSize(size_);
    GetTransform()->SetTexSize(size_);
    GetTransform()->SetPivot(size_ * 0.5f);

    SetMoveSpeed(500.0f);
}

// 更新
void EnemyMadScientist::Update(const float& elapsedTime)
{
    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);
}

// ImGui
void EnemyMadScientist::DrawDebug()
{
    const std::string name = GetName() + std::to_string(GetObjectId());
    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed))
    {
        if (ImGui::Button("SummonState")) ChangeState(State::Summon);
        if (ImGui::Button("HideState")) ChangeState(State::Hide);

        Object::DrawDebug();

        ImGui::TreePop();
    }
}

void EnemyMadScientist::OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position)
{

}

// 旋回処理
void EnemyMadScientist::Turn()
{
    //GetTransform()->SetAngle(DirectX::XMConvertToDegrees(atan2f(moveDirection_.y, moveDirection_.x) + DirectX::XM_PIDIV2));
}
