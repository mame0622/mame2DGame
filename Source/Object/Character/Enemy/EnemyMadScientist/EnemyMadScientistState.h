#pragma once
#include "Ai/StateMachine/State.h"
#include "Object/Character/Enemy/Enemy.h"

namespace EnemyMadScientistState
{
    class DecisionState : public State<Enemy>
    {
    public:
        DecisionState(Enemy* enemy) : State(enemy, "DecisionState") {}
        ~DecisionState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {}
        void DrawDebug()                        override;
    };

    class SummonState : public State<Enemy>
    {
    public:
        SummonState(Enemy* enemy) : State(enemy, "SummonState") {}
        ~SummonState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {}
        void DrawDebug()                        override;

    private:
        float length_ = 300.0f;

        const float recoveryTime_   = 2.0f;
        float       recoveryTimer_  = 0.0f;
    };

    class HideState : public State<Enemy>
    {
    public:
        HideState(Enemy* enemy) : State(enemy, "HideState") {}
        ~HideState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {}
        void DrawDebug()                        override;

    private:
        const bool CalcHidePosition();

    private:
        DirectX::XMFLOAT2 hidePosition_ = {};
        float hideLength_ = 300.0f;
    };

    class RetreatState : public State<Enemy>
    {
    public:
        RetreatState(Enemy* enemy) : State(enemy, "RetreatState") {}
        ~RetreatState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {}
        void DrawDebug()                        override;
    };
}
