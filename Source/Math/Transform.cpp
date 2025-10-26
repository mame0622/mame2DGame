#include "Transform.h"
#include "ImGui/ImGuiCtrl.h"

#pragma region ---------- Transform2D ----------
// ----- ImGui -----
void Transform2D::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::TreeNodeEx("Transform2D", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat2("Position", &position_.x);
        ImGui::DragFloat2("Size", &size_.x);
        ImGui::ColorEdit4("Color", &color_.x);
        ImGui::DragFloat2("TexPos", &texPos_.x);
        ImGui::DragFloat2("TexSize", &texSize_.x);
        ImGui::DragFloat2("Pivot", &pivot_.x);
        ImGui::DragFloat("Angle", &angle_);

        ImGui::TreePop();
    }

#endif// USE_IMGUI
}

#pragma endregion ---------- Transform2D ----------