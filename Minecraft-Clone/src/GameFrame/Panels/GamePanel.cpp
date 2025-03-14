

#include "GamePanel.h"



void GamePanel::SetImguiProperties() {
	ImGui::Begin("Players");

	glm::vec3 pos = m_GameWorld.GetPlayerPosition();
	ImGui::Text("Position:- ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Position", glm::value_ptr(pos));

	ImGui::End();

	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}

void GamePanel::UnSetImguiProperties() {
	ImGui::PopStyleVar();
}

void GamePanel::OnUpdate() {
	m_GameWorld.OnUpdate();
}

void GamePanel::OnImguiUiUpdate() {
	CheckAndHandleResize();
	RenderViewport();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImVec2 imagePos = ImGui::GetCursorPos();
	ImTextureID frameBuffer = (ImTextureID)(intptr_t)m_Frame->GetColorAttachment(0);
	bool clickedOnImage = ImGui::ImageButton(
		"viewportimage",
		frameBuffer,
		ImVec2((float)m_PanelWidth, (float)m_PanelHeight),
		{ 0, 1 },
		{ 1, 0 }
	);

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}

void GamePanel::CheckAndHandleResize() {
	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	if (windowSize.x < 0 || windowSize.y < 0) {
		return;
	}

	if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
		m_PanelWidth  = (int)windowSize.x;
		m_PanelHeight = (int)windowSize.y;

		m_GameWorld.HandleResize(m_PanelWidth, m_PanelHeight);

		m_Frame->Resize(m_PanelWidth, m_PanelHeight);
		Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
	}
}

void GamePanel::RenderViewport() {
	m_Frame->Bind();
	m_Frame->ClearAttachments();

	m_GameWorld.RenderWorld();

	m_Frame->Unbind();
}

bool GamePanel::OnEvent(Quirk::Event& event) {
	return m_GameWorld.OnEvent(event);
}
