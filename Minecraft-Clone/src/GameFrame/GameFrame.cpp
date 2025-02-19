

#include "GameFrame.h"
#include "FontManager.h"
#include "Theme.h"


bool GameFrame::OnEvent(Quirk::Event& event){
	return false;
}

void GameFrame::OnUpdate(){

}

void GameFrame::OnImguiUiUpdate(){
	// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
	ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
}

