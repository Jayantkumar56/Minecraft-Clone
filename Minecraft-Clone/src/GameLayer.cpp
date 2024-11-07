

#include "GameLayer.h"


GameLayer::GameLayer() : Layer("GameLayer") {

}

void GameLayer::OnAttach() {

}

void GameLayer::OnDetach() {

}

bool GameLayer::OnEvent(Quirk::Event& event) {
	m_Controller.OnEvent(event);

	return false;
}

void GameLayer::OnUpdate() {
	m_Controller.OnUpdate();

	Quirk::Renderer::BeginScene(m_Controller.GetProjectionViewMatrix());

	m_World.SubmitMesh();
	
	Quirk::Renderer::EndScene();
}

void GameLayer::OnImguiUiUpdate() {

}
