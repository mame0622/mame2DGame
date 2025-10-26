#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "Graphics/FrameBuffer/FrameBuffer.h"
#include "Graphics/FullscreenQuad/FullscreenQuad.h"

class PostProcess
{
private:
    PostProcess();
    ~PostProcess() {}

public:
    static PostProcess& Instance()
    {
        static PostProcess instance;
        return instance;
    }

    void Activate();
    void Deactivate();
    void Draw();

    void DrawDebug();

private:
    std::unique_ptr<FullscreenQuad> renderer_;
    std::unique_ptr<FrameBuffer>    sceneBuffer_;
    std::unique_ptr<FrameBuffer>    postProcess_;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> postProcessPS_;
};

