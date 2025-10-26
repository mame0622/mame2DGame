#pragma once
#include "MathHelper.h"
#include <string>

class Transform2D
{
public:
    Transform2D() {}
    ~Transform2D() {}

    void DrawDebug();

    // ---------- Position ----------
    [[nodiscard]] const DirectX::XMFLOAT2 GetPosition() const { return position_; }
    [[nodiscard]] const float GetPositionX() const { return position_.x; }
    [[nodiscard]] const float GetPositionY() const { return position_.y; }
    void SetPosition(const DirectX::XMFLOAT2& position) { position_ = position; }
    void SetPosition(const float& x, const float& y) { position_ = { x, y }; }
    void SetPositionX(const float& x) { position_.x = x; }
    void SetPositionY(const float& y) { position_.y = y; }
    void AddPosition(const DirectX::XMFLOAT2& position) { position_ += position; }
    void AddPosition(const float& x, const float& y) { position_ += { x, y }; }
    void AddPositionX(const float& x) { position_.x += x; }
    void AddPositionY(const float& y) { position_.y += y; }

    // ---------- Size ----------
    [[nodiscard]] const DirectX::XMFLOAT2 GetSize() const { return size_; }
    [[nodiscard]] const float GetSizeX() const { return size_.x; }
    [[nodiscard]] const float GetSizeY() const { return size_.y; }
    void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
    void SetSize(const float& x, const float& y) { size_ = { x, y }; }
    void SetSize(const float& size) { size_ = { size, size }; }
    void SetSizeX(const float& x) { size_.x = x; }
    void SetSizeY(const float& y) { size_.y = y; }

    // ---------- CenterPosition ----------
    [[nodiscard]] const DirectX::XMFLOAT2 GetCenterPosition() { return position_ + size_ * 0.5f; }
    [[nodiscard]] const float GetCenterPositionX() { return position_.x + size_.x * 0.5f; }
    [[nodiscard]] const float GetCenterPositionY() { return position_.y + size_.y * 0.5f; }

    // ---------- Color ----------
    [[nodiscard]] const DirectX::XMFLOAT4 GetColor() const { return color_; }
    [[nodiscard]] const float GetColorR() const { return color_.x; }
    [[nodiscard]] const float GetColorG() const { return color_.y; }
    [[nodiscard]] const float GetColorB() const { return color_.z; }
    [[nodiscard]] const float GetColorA() const { return color_.w; }
    void SetColor(const DirectX::XMFLOAT4& color) { color_ = color; }
    void SetColor(const DirectX::XMFLOAT3& color) { color_ = { color.x, color.y, color.z, color_.w }; }
    void SetColor(const float& r, const float& g, const float& b, const float& a) { color_ = { r, g, b, a }; }
    void SetColor(const float& r, const float& g, const float& b) { color_ = { r, g, b, color_.w }; }
    void SetColorR(const float& r) { color_.x = r; }
    void SetColorG(const float& g) { color_.y = g; }
    void SetColorB(const float& b) { color_.z = b; }
    void SetColorA(const float& a) { color_.w = a; }

    // ---------- texPos ----------
    [[nodiscard]] const DirectX::XMFLOAT2 GetTexPos() const { return texPos_; }
    [[nodiscard]] const float GetTexPosX() const { return texPos_.x; }
    [[nodiscard]] const float GetTexPosY() const { return texPos_.y; }
    void SetTexPos(const DirectX::XMFLOAT2& texPos) { texPos_ = texPos; }
    void SetTexPos(const float& x, const float& y) { texPos_ = { x, y }; }
    void SetTexPosX(const float& x) { texPos_.x = x; }
    void SetTexPosY(const float& y) { texPos_.y = y; }

    // ---------- texSize ----------
    [[nodiscard]] const DirectX::XMFLOAT2 GetTexSize() const { return texSize_; }
    [[nodiscard]] const float GetTexSizeX() const { return texSize_.x; }
    [[nodiscard]] const float GetTexSizeY() const { return texSize_.y; }
    void SetTexSize(const DirectX::XMFLOAT2& texSize) { texSize_ = texSize; }
    void SetTexSize(const float& x, const float& y) { texSize_ = { x, y }; }
    void SetTexSize(const float& texSize) { texSize_ = { texSize, texSize }; }
    void SetTexSizeX(const float& x) { texSize_.x = x; }
    void SetTexSizeY(const float& y) { texSize_.y = y; }

    // ---------- pivot ----------
    [[nodiscard]] const DirectX::XMFLOAT2 GetPivot() const { return pivot_; }
    [[nodiscard]] const float GetPivotX() const { return pivot_.x; }
    [[nodiscard]] const float GetPivotY() const { return pivot_.y; }
    void SetPivot(const DirectX::XMFLOAT2& pivot) { pivot_ = pivot; }
    void SetPivot(const float& pivot) { pivot_ = { pivot, pivot }; }
    void SetPivot(const float& x, const float& y) { pivot_ = { x, y }; }
    void SetPivotX(const float& x) { pivot_.x = x; }
    void SetPivotY(const float& y) { pivot_.y = y; }

    // ---------- angle ----------
    [[nodiscard]] const float GetAngle() const { return angle_; }
    void SetAngle(const float& angle) { angle_ = angle; }
    void AddAngle(const float& angle) { angle_ += angle; }

private:
    DirectX::XMFLOAT2   position_   = {};                           // 位置
    DirectX::XMFLOAT2   size_       = { 100.0f, 100.0f };           // 大きさ
    DirectX::XMFLOAT4   color_      = { 1.0f, 1.0f, 1.0f, 1.0f };   // 色
    DirectX::XMFLOAT2   texPos_     = {};                           // UV座標
    DirectX::XMFLOAT2   texSize_    = { 100.0f, 100.0f };           // UVサイズ
    DirectX::XMFLOAT2   pivot_      = {};                           // 回転中心点
    float               angle_      = 0.0f;                         // 角度
};