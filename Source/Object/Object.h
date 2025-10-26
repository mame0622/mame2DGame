#pragma once
#include "Math/Transform.h"
#include "Collision/Collision.h"

class Object
{
public:
    Object(const std::string& name);
    virtual ~Object() {}

    virtual void DrawDebug(); // ImGui

    virtual void OnHit(const Collision::Type& type, const DirectX::XMFLOAT2& position) {}

    Transform2D* GetTransform() { return &transform_; }
    const std::string GetName() const { return name_; }
    const int GetObjectId() const { return objectId_; }

private:
    Transform2D         transform_;
    const std::string   name_;
    const int           objectId_   = 0;    
    static int          objectIdCount_;
};

