

#pragma once

#include "World/World.h"
#include "Quirk.h"

class GameLayer : public Quirk::Layer {
public:
	GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual bool OnEvent(Quirk::Event& event) override;
	virtual void OnUpdate() override;
	virtual void OnImguiUiUpdate() override;

private:
	World m_World;
	Quirk::PerspectiveCameraController m_Controller;
};
