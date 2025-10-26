#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "Math/Transform.h"

class Sprite
{
public:
    Sprite(const wchar_t* filename);
    ~Sprite() {}

    void Render();
    void DrawDebug();

    struct Vertex
    {
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT4 color_;
        DirectX::XMFLOAT2 texcoord_;
    };

public:
    Transform2D* GetTransform() { return &transform_; }

private:
    void Rotate(float& x, float& y, const float& centerX, const float& centerY, const float& angle);

private:
    Transform2D transform_ = {};

    // ---------- シェーダー ----------
    Microsoft::WRL::ComPtr<ID3D11VertexShader>          vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>           pixelShader_;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>           inputLayout_;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                vertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    shaderResourceView_;
};

