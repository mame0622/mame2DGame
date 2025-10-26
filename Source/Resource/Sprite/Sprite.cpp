#include "Sprite.h"
#include "Application/Misc.h"
#include "Graphics/Graphics.h"
#include "Resource/Texture/Texture.h"

Sprite::Sprite(const wchar_t* filename)
{
    // 頂点情報のセット
    Vertex vertices[]
    {
        { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
        { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
        { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
        { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth            = sizeof(vertices);
    bufferDesc.Usage                = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags            = 0;
    bufferDesc.StructureByteStride  = 0;

    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem             = vertices;
    subresourceData.SysMemPitch         = 0;
    subresourceData.SysMemSlicePitch    = 0;

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
    D3D11_TEXTURE2D_DESC texture2dDesc  = {};
    Texture::TextureData textureData    = Texture::Instance().LoadTexture(filename);
    shaderResourceView_ = textureData.shaderResourceView_;
    texture2dDesc       = textureData.texture2dDesc_;

    // テクスチャ情報から、各種項目を初期設定する
    const float width   = static_cast<float>(texture2dDesc.Width);
    const float height  = static_cast<float>(texture2dDesc.Height);
    transform_.SetSize(width, height);
    transform_.SetTexSize(width, height);
    transform_.SetPivot(transform_.GetSize() * 0.5f);
}

// 描画
void Sprite::Render()
{
    ID3D11DeviceContext* deviceContext = Graphics::Instance().GetDeviceContext();
    D3D11_VIEWPORT          viewport = {};
    UINT                    numViewports = 1;

    // スクリーンサイズを取得
    deviceContext->RSGetViewports(&numViewports, &viewport);

    // leftTop
    float x0 = transform_.GetPositionX();
    float y0 = transform_.GetPositionY();
    // rightTop
    float x1 = transform_.GetPositionX() + transform_.GetSizeX();
    float y1 = transform_.GetPositionY();
    // leftBottom
    float x2 = transform_.GetPositionX();
    float y2 = transform_.GetPositionY() + transform_.GetSizeY();
    // rightBottom
    float x3 = transform_.GetPositionX() + transform_.GetSizeX();
    float y3 = transform_.GetPositionY() + transform_.GetSizeY();

    // 回転処理
    Rotate(x0, y0, transform_.GetPositionX() + transform_.GetPivotX(), transform_.GetPositionY() + transform_.GetPivotY(), transform_.GetAngle());
    Rotate(x1, y1, transform_.GetPositionX() + transform_.GetPivotX(), transform_.GetPositionY() + transform_.GetPivotY(), transform_.GetAngle());
    Rotate(x2, y2, transform_.GetPositionX() + transform_.GetPivotX(), transform_.GetPositionY() + transform_.GetPivotY(), transform_.GetAngle());
    Rotate(x3, y3, transform_.GetPositionX() + transform_.GetPivotX(), transform_.GetPositionY() + transform_.GetPivotY(), transform_.GetAngle());

    // スクリーン座標系から NDCへの座標変換をおこなう
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    // 計算結果で頂点バッファオブジェクトを更新する
    HRESULT                     result = S_OK;
    D3D11_MAPPED_SUBRESOURCE    mappedSubresouce = {};
    result = deviceContext->Map(vertexBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresouce);
    _ASSERT_EXPR(SUCCEEDED(result), HRTrace(result));

    Vertex* vertices = reinterpret_cast<Vertex*>(mappedSubresouce.pData);
    if (vertices != nullptr)
    {
        vertices[0].position_ = { x0, y0, 0 };
        vertices[1].position_ = { x1, y1, 0 };
        vertices[2].position_ = { x2, y2, 0 };
        vertices[3].position_ = { x3, y3, 0 };
        vertices[0].color_ = vertices[1].color_ = vertices[2].color_ = vertices[3].color_ = transform_.GetColor();

        const DirectX::XMFLOAT2 texPos = transform_.GetTexPos();
        const DirectX::XMFLOAT2 texSize = transform_.GetTexSize();
        const DirectX::XMFLOAT2 size = transform_.GetSize();

        vertices[0].texcoord_ = { texPos.x / size.x, texPos.y / size.y };
        vertices[1].texcoord_ = { (texPos.x + texSize.x) / size.x, texPos.y / size.y };
        vertices[2].texcoord_ = { texPos.x / size.x, (texPos.y + texSize.y) / size.y };
        vertices[3].texcoord_ = { (texPos.x + texSize.x) / size.x, (texPos.y + texSize.y) / size.y };
    }
    deviceContext->Unmap(vertexBuffer_.Get(), 0);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    deviceContext->IASetInputLayout(inputLayout_.Get());

    deviceContext->VSSetShader(vertexShader_.Get(), nullptr, 0);

    deviceContext->PSSetShader(pixelShader_.Get(), nullptr, 0);

    deviceContext->PSSetShaderResources(0, 1, shaderResourceView_.GetAddressOf());

    deviceContext->Draw(4, 0);
}

// ImGui
void Sprite::DrawDebug()
{
    transform_.DrawDebug();
}

// 回転処理
void Sprite::Rotate(float& x, float& y, const float& centerX, const float& centerY, const float& angle)
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
