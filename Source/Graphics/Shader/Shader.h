#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class Shader
{
public:
    enum class BlendState { None, Alpha, Add, Multiply, MRT, Max };
    enum class RasterState { Solid, Wireframe, CullNone, WireframeCullNone, Max };
    enum class DepthState { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF, Max };
    enum class SamplerState { Point, Linear, Anisotropic, LinearBorderBlack, LinearBorderWhite, Comparison, Max };

public:
    Shader(ID3D11Device* device);
    ~Shader() {}

    // ---------- シェーダーオブジェクト生成関数 ----------
    HRESULT CreateVsFromCso(ID3D11Device* device, const char* csoName, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements);
    HRESULT CreatePsFromCso(ID3D11Device* device, const char* csoName, ID3D11PixelShader** pixelShader);
    HRESULT CreateGsFromCso(ID3D11Device* device, const char* csoName, ID3D11GeometryShader** geometryShader);
    HRESULT CreateCsFromCso(ID3D11Device* device, const char* csoName, ID3D11ComputeShader** computeShader);
    HRESULT CreateDsFromCso(ID3D11Device* device, const char* csoName, ID3D11DomainShader** domainShader);
    HRESULT CreateHsFromCso(ID3D11Device* device, const char* csoName, ID3D11HullShader** hullShader);

    void SetBlendState(ID3D11DeviceContext* deviceContext, const BlendState& blendState) { deviceContext->OMSetBlendState(blendStates_[static_cast<UINT>(blendState)].Get(), nullptr, 0xFFFFFFFF); }
    void SetRasterizerState(ID3D11DeviceContext* deviceContext, const RasterState& rasterizerState) { deviceContext->RSSetState(rasterizerStates_[static_cast<UINT>(rasterizerState)].Get()); }
    void SetDepthStencileState(ID3D11DeviceContext* deviceContext, const DepthState& depthStencileState) { deviceContext->OMSetDepthStencilState(depthStencilStates_[static_cast<UINT>(depthStencileState)].Get(), 1); }
    void SetSamplerState(ID3D11DeviceContext* deviceContext);

private:
    void CreateBlendStates(ID3D11Device* device);           // ブレンドステート作成
    void CreateRasterizerStates(ID3D11Device* device);      // ラスタライザステート作成
    void CreateDepthStencilStates(ID3D11Device* device);    // デプスステンシルステート作成
    void CreateSamplerStates(ID3D11Device* device);         // サンプラーステート作成

private:
    Microsoft::WRL::ComPtr<ID3D11BlendState>        blendStates_[static_cast<int>(BlendState::Max)];
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerStates_[static_cast<int>(RasterState::Max)];
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates_[static_cast<int>(DepthState::Max)];
    Microsoft::WRL::ComPtr<ID3D11SamplerState>      samplerStates_[static_cast<int>(SamplerState::Max)];
};