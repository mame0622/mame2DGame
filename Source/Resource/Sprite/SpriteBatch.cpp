#include "SpriteBatch.h"
#include <memory>
#include "Application/Misc.h"
#include "Graphics/Graphics.h"
#include "Resource/Texture/Texture.h"

SpriteBatch::SpriteBatch(const wchar_t* filename, const size_t& maxSprites)
    : maxVertices_(maxSprites * 6)
{
    std::unique_ptr<Vertex[]> vertices = std::make_unique<Vertex[]>(maxVertices_);

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(maxVertices_);
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem = vertices.get();
    subresourceData.SysMemPitch = 0;
    subresourceData.SysMemSlicePitch = 0;

    HRESULT result = S_OK;
    result = Graphics::Instance().GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer_.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    // 入力レイアウトオブジェクト
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // シェーダー読み込み
    Graphics::Instance().CreateVsFromCso("./Resources/Shader/SpriteVS.cso", vertexShader_.GetAddressOf(), inputLayout_.GetAddressOf(), inputElementDesc, _countof(inputElementDesc));
    Graphics::Instance().CreatePsFromCso("./Resources/Shader/SpritePS.cso", pixelShader_.GetAddressOf());

    // テクスチャ読み込み
    D3D11_TEXTURE2D_DESC texture2dDesc = {};
    Texture::TextureData textureData = Texture::Instance().LoadTexture(filename);
    shaderResourceView_ = textureData.shaderResourceView_;
    texture2dDesc = textureData.texture2dDesc_;

    // テクスチャ情報から、各種項目を初期設定する
    const float width = static_cast<float>(texture2dDesc.Width);
    const float height = static_cast<float>(texture2dDesc.Height);
    transform_.SetSize(width, height);
    transform_.SetTexSize(width, height);
    transform_.SetPivot(transform_.GetSize() * 0.5f);
}

// 描画
void SpriteBatch::Render(const std::vector<Transform2D>& transforms)
{
    Begin();

    const size_t maxTransforms = transforms.size();
    for (int i = 0; i < maxTransforms; ++i)
    {
        Render(transforms.at(i));
    }

    End();
}

// 描画 ( 内部処理 )
void SpriteBatch::Render(const Transform2D& transform)
{
    ID3D11DeviceContext* deviceContext = Graphics::Instance().GetDeviceContext();
    D3D11_VIEWPORT          viewport = {};
    UINT                    numViewports = 1;

    // スクリーンサイズを取得
    deviceContext->RSGetViewports(&numViewports, &viewport);

    // leftTop
    float x0 = transform.GetPositionX();
    float y0 = transform.GetPositionY();
    // rightTop
    float x1 = transform.GetPositionX() + transform.GetSizeX();
    float y1 = transform.GetPositionY();
    // leftBottom
    float x2 = transform.GetPositionX();
    float y2 = transform.GetPositionY() + transform.GetSizeY();
    // rightBottom
    float x3 = transform.GetPositionX() + transform.GetSizeX();
    float y3 = transform.GetPositionY() + transform.GetSizeY();

    // 回転処理
    Rotate(x0, y0, transform.GetPositionX() + transform.GetPivotX(), transform.GetPositionY() + transform.GetPivotY(), transform.GetAngle());
    Rotate(x1, y1, transform.GetPositionX() + transform.GetPivotX(), transform.GetPositionY() + transform.GetPivotY(), transform.GetAngle());
    Rotate(x2, y2, transform.GetPositionX() + transform.GetPivotX(), transform.GetPositionY() + transform.GetPivotY(), transform.GetAngle());
    Rotate(x3, y3, transform.GetPositionX() + transform.GetPivotX(), transform.GetPositionY() + transform.GetPivotY(), transform.GetAngle());

    // スクリーン座標系から NDCへの座標変換をおこなう
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    float u0 = transform.GetTexPosX() / transform.GetSizeX();
    float v0 = transform.GetTexPosY() / transform.GetSizeY();
    float u1 = (transform.GetTexPosX() + transform.GetTexSizeX()) / transform.GetSizeX();
    float v1 = (transform.GetTexPosY() + transform.GetTexSizeY()) / transform.GetSizeY();

    vertices_.push_back({ { x0, y0, 0 }, transform.GetColor(), { u0, v0 } });
    vertices_.push_back({ { x1, y1, 0 }, transform.GetColor(), { u1, v0 } });
    vertices_.push_back({ { x2, y2, 0 }, transform.GetColor(), { u0, v1 } });
    vertices_.push_back({ { x2, y2, 0 }, transform.GetColor(), { u0, v1 } });
    vertices_.push_back({ { x1, y1, 0 }, transform.GetColor(), { u1, v0 } });
    vertices_.push_back({ { x3, y3, 0 }, transform.GetColor(), { u1, v1 } });
}

// 描画準備
void SpriteBatch::Begin()
{
    ID3D11DeviceContext* deviceContext = Graphics::Instance().GetDeviceContext();

    vertices_.clear();
    deviceContext->VSSetShader(vertexShader_.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader_.Get(), nullptr, 0);
    deviceContext->PSSetShaderResources(0, 1, shaderResourceView_.GetAddressOf());
}

// 描画終了処理
void SpriteBatch::End()
{
    HRESULT result = S_OK;
    ID3D11DeviceContext* deviceContext = Graphics::Instance().GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
    result = deviceContext->Map(vertexBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    size_t vertexCount = vertices_.size();
    _ASSERT_EXPR(maxVertices_ >= vertexCount, "Buffer overflow");
    Vertex* data = reinterpret_cast<Vertex*>(mappedSubresource.pData);
    if (data != nullptr)
    {
        const Vertex* p = vertices_.data();
        memcpy_s(data, maxVertices_ * sizeof(Vertex), p, vertexCount * sizeof(Vertex));
    }
    deviceContext->Unmap(vertexBuffer_.Get(), 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->IASetInputLayout(inputLayout_.Get());

    deviceContext->Draw(static_cast<UINT>(vertexCount), 0);
}

// 回転処理
void SpriteBatch::Rotate(float& x, float& y, const float& centerX, const float& centerY, const float& angle)
{
    // 指定の位置に動かす
    x -= centerX;
    y -= centerY;

    // 回転処理
    float cos = cosf(DirectX::XMConvertToRadians(angle));
    float sin = sinf(DirectX::XMConvertToRadians(angle));
    float tx = x;
    float ty = y;
    x = cos * tx + -sin * ty;
    y = sin * tx + cos * ty;

    // 元の位置に戻す
    x += centerX;
    y += centerY;
}