

#pragma once
#include "Quirk.h"

class World;

class Player {
	friend class World;

public:
	Player() : m_CameraController(glm::radians(45.0f), 1.0f, 1.0f, 1000.0f) { }

	void OnUpdate() { m_CameraController.OnUpdate(); }

	bool OnEvent(Quirk::Event& event) { 
		Quirk::EventDispatcher::HandleEvent<Quirk::KeyPressedEvent>([&] (Quirk::KeyPressedEvent event) -> bool {
			if (event.GetKeyCode() == QK_Key_Y) {
				m_HaveFocus  = true;
				auto& window = Quirk::Application::GetFrameManager().GetCurrentFrame()->GetWindow();
				window.HideCursor();
				window.SetCursorAtCenter();
				window.SetCursorLocked(true);
	
			}
			if (event.GetKeyCode() == QK_Key_Escape) {
				m_HaveFocus  = false;
				auto& window = Quirk::Application::GetFrameManager().GetCurrentFrame()->GetWindow();
				window.ShowCursor();
				window.SetCursorLocked(false);
			}

			return false;
		});

		if (m_HaveFocus) 
			m_CameraController.OnEvent(event);

		return false;
	}

	glm::vec3 GetPosition() { return m_CameraController.GetPosition(); }

private:
	bool m_HaveFocus = false;
	Quirk::PerspectiveCameraController m_CameraController;
};
