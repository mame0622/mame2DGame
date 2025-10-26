#pragma once
#include "Player.h"
#include <memory>

class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    void Initialize();
    void Update(const float& elapsedTime);
    void Render();
    void DrawDebug();

    Transform2D* GetTransform() { return player_->GetTransform(); }
    std::unique_ptr<Player>& GetPlayer() { return player_; }

private:
    std::unique_ptr<Player> player_;
};

