#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "Math/Transform.h"

class SpriteBatch
{
public:
    SpriteBatch(const wchar_t* filename, const size_t& maxSprites);
    ~SpriteBatch() {}

    void Render(const std::vector<Transform2D>& transforms);

    struct Vertex
    {
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT4 color_;
        DirectX::XMFLOAT2 texcoord_;
    };

    // 読み取り専用
    Transform2D GetTransform() { return transform_; }

private:
    void Render(const Transform2D& transform);
    void Begin();
    void End();
    void Rotate(float& x, float& y, const float& centerX, const float& centerY, const float& angle);

private:
    const size_t        maxVertices_;
    std::vector<Vertex> vertices_;

    Transform2D transform_ = {};

    // ---------- シェーダー ----------
    Microsoft::WRL::ComPtr<ID3D11VertexShader>          vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>           pixelShader_;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>           inputLayout_;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                vertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    shaderResourceView_;
};

