

#include "GamePanel.h"



void GamePanel::SetImguiProperties() {
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
}

void GamePanel::OnUpdate() {

}

void GamePanel::OnImguiUiUpdate() {

}

bool GamePanel::OnEvent(Quirk::Event& event) {

    return false;
}
