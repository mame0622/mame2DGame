#pragma once
#include "Ai/StateMachine/State.h"
#include "Object/Character/Enemy/Enemy.h"

namespace EnemyNecromancerState
{
    class PursuitState : public State<Enemy>
    {
    public:
        PursuitState(Enemy* enemy) : State(enemy, "PursuitState") {}
        ~PursuitState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {}
        void DrawDebug()                        override;

    private:
        // ---------- PursuitTimer ----------
        const float pursuitTime_    = 3.0f;
        float       pursuitTimer_   = 0.0f;

        // ---------- Move ----------
        const float moveSpeed_ = 100.0f;
    };

    class NecromancyState : public State<Enemy>
    {
    public:
        NecromancyState(Enemy* enemy) : State(enemy, "NecromancyState") {}
        ~NecromancyState() {}

        void Initialize()                       override;
        void Update(const float& elapsedTime)   override;
        void Finalize()                         override {}
        void DrawDebug()                        override;

    private:
        void UndeadGeneration(); // EnemyNormalÇê∂ê¨Ç∑ÇÈ

    private:
        // ---------- GenerationTimer ----------
        const float generationTime_     = 2.0f;
        float       generationTimer_    = 0.0f;

        // ---------- Move ----------
        const float moveSpeed_ = 0.0f;
    };
};

