#include "Application.h"
#include "Scene/SceneManager.h"
#include "Graphics/PostProcess/PostProcess.h"

Application::Application(HWND hwnd)
    : hwnd_(hwnd), graphics_(hwnd), input_(hwnd)
{
}

// 初期化
const bool Application::Initialize()
{
    // Input初期設定
    input_.GetMouse().SetScreenWidth(SCREEN_WIDTH);
    input_.GetMouse().SetScreenHeight(SCREEN_HEIGHT);

    // Scene初期化
    SceneManager::Instance().Initilaize();

    return true;
}

// 終了化
const bool Application::Finalize()
{
    // Scene終了化
    SceneManager::Instance().Finalize();

    return false;
}

// 更新
void Application::Update(const float& elapsedTime)
{
    // ImGui更新
    IMGUI_CTRL_CLEAR_FRAME();

    // 入力更新
    input_.Update();

    // Scene更新
    SceneManager::Instance().Update(elapsedTime);

    // ImGui
    DrawDebug();

    if (input_.GetGamePad().GetButtonDown() & GamePad::BTN_UP) isImGuiActive_ = !isImGuiActive_;
}

// 描画
void Application::Render()
{
    // 描画初期化
    graphics_.RenderInitialize();

    PostProcess::Instance().Activate();

    // Scene描画
    SceneManager::Instance().Render();

    PostProcess::Instance().Deactivate();
    PostProcess::Instance().Draw();

    // ImGui描画
    IMGUI_CTRL_DISPLAY();

    // 描画実行
    graphics_.Draw();
}

// ImGui
void Application::DrawDebug()
{
#if USE_IMGUI
    if (isImGuiActive_ == false) return;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("MainDockSpace", nullptr, windowFlags);
    ImGui::PopStyleVar(2);

    ImGuiID dockSpaceID = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();

    // Scene
    SceneManager::Instance().DrawDebug();

    PostProcess::Instance().DrawDebug();

#endif // USE_IMGUI
}

// 実行
const int Application::Run()
{
    MSG msg = {};

    if (Initialize() == false) return 0;

    // ImGui初期化(DirectX11の初期化の下に置くこと)
    IMGUI_CTRL_INITIALIZE(hwnd_, graphics_.GetDevice(), graphics_.GetDeviceContext());

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            tictoc_.Tick();
            CalculateFrameStats();
            Update(tictoc_.TimeInterval());
            Render();
        }
    }

    // ImGui終了化
    IMGUI_CTRL_UNINITIALIZE();

    BOOL fullscreen = 0;
    graphics_.GetSwapChain()->GetFullscreenState(&fullscreen, 0);
    if (fullscreen)
    {
        graphics_.GetSwapChain()->SetFullscreenState(FALSE, 0);
    }

    return Finalize() ? static_cast<int>(msg.wParam) : 0;
}

LRESULT Application::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // ImGui
    IMGUI_CTRL_WND_PRC_HANDLER(hwnd, msg, wparam, lparam);

    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps{};
        BeginPaint(hwnd, &ps);

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        break;
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        break;
    case WM_ENTERSIZEMOVE:
        tictoc_.Stop();
        break;
    case WM_EXITSIZEMOVE:
        tictoc_.Start();
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

// フレーム計算
void Application::CalculateFrameStats()
{
    if (++frames_, (tictoc_.TimeStamp() - elapsedTime_) >= 1.0f)
    {
        float fps = static_cast<float>(frames_);
        std::wostringstream outs;
        outs.precision(6);

        // ゲームタイトル
        outs << APPLICATION_NAME;

        // FPS
        outs << L" FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";

        SetWindowTextW(hwnd_, outs.str().c_str());

        frames_ = 0;
        elapsedTime_ += 1.0f;
    }
}
