#include "PostProcess.h"
#include "Graphics/Graphics.h"
#include "ImGui/imgui.h"

PostProcess::PostProcess()
{
    Graphics::Instance().CreatePsFromCso("./Resources/Shader/PostProcessPS.cso", postProcessPS_.GetAddressOf());

    renderer_ = std::make_unique<FullscreenQuad>();

    sceneBuffer_ = std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
    postProcess_ = std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void PostProcess::Activate()
{
    sceneBuffer_->Clear(0,1,1,1);
    sceneBuffer_->Activate();
}

void PostProcess::Deactivate()
{
    sceneBuffer_->Deactivate();
}

void PostProcess::Draw()
{
    Graphics::Instance().SetDepthStencileState(Shader::DepthState::ZT_OFF_ZW_OFF);
    Graphics::Instance().SetRasterizerState(Shader::RasterState::CullNone);
    Graphics::Instance().SetBlendState(Shader::BlendState::None);

    postProcess_->Clear();
    postProcess_->Activate();

    renderer_->Draw(sceneBuffer_->GetColorMap().GetAddressOf(), 0, 1, postProcessPS_.Get());

    postProcess_->Deactivate();

    renderer_->Draw(postProcess_->GetColorMap().GetAddressOf(), 0, 1, postProcessPS_.Get());
}

// ImGui
void PostProcess::DrawDebug()
{
    // ---------- Scene 描画表示 ----------
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    const ImVec2 contentSize = ImGui::GetContentRegionAvail();

    // 1280 x 720 のアスペクト比を保つ
    const float targetAspect = 1280.0f / 720.0f;
    const float windowAspect = contentSize.x / contentSize.y;

    ImVec2 drawSize = {};

    if (windowAspect > targetAspect)
    {
        drawSize.y = contentSize.y;
        drawSize.x = drawSize.y * targetAspect;
    }
    else
    {
        drawSize.x = contentSize.x;
        drawSize.y = drawSize.x / targetAspect;
    }

    // 中央配置
    const ImVec2 cursorPos = ImGui::GetCursorPos();
    const ImVec2 setPosition = ImVec2(cursorPos.x + (contentSize.x - drawSize.x) * 0.5f, cursorPos.y + (contentSize.y - drawSize.y) * 0.5f);
    ImGui::SetCursorPos(setPosition);

    ImGui::Image(reinterpret_cast<ImTextureID>(postProcess_->GetColorMap().Get()), drawSize);

    ImGui::End();// Scene
}
