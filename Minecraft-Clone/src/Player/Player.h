

#pragma once
#include "Quirk.h"

#include "CameraController.h"

class World;

class Player {
	friend class World;

public:
	Player();

	void OnUpdate() { m_CameraController.OnUpdate(); }

	const auto& GetCamera()   const { return m_CameraController.GetCamera();   }
	const auto  GetPosition() const { return m_CameraController.GetPosition(); }

	void SetControl(bool flag) { m_IsInControl = flag; }

private:
	bool m_IsInControl = false;
	CameraController m_CameraController;
};
