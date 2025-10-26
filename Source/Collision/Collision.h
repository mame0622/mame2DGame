#pragma once
class Object;
class Transform2D;
#include <DirectXMath.h>

class Collision
{
public:
    enum class Type
    {
        Player,
        Enemy,
        BulletOrbit,
        Bullet,

        Max,
    };

    Collision(const Type& type, Object* owner, const float& radius);
    ~Collision() {}

    void Remove();

    void OnHit(const Type& type, const DirectX::XMFLOAT2& position);

    const Type GetType() const { return type_; }
    Transform2D* GetTransform();
    const float GetRadius() const { return radius_; }

private:
    const Type  type_;
    Object*     owner_;     // éùÇøéÂ
    const float radius_;    // ìñÇΩÇËîªíËîºåa

    bool isActive_ = true;
};