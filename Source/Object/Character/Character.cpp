#include "Character.h"
#include "ImGui/ImGuiCtrl.h"

Character::Character(const std::string& name)
    : Object(name)
{
}

void Character::DrawDebug()
{
#if USE_IMGUI
#endif // USE_IMGUI
}
