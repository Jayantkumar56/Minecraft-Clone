

#pragma once

#include "Camera.h"
#include "Quirk.h"


struct CameraControls {
	int Forward;
	int Backward;
	int Right;
	int Left;
	int Up;
	int Down;
};

struct ControllerSpecs {
	float MaxSpeed;
	float Acceleration;
	float Deceleration;
};

class CameraController {
public:
	CameraController(const PerspectiveCameraSpecs& camSpec, const CameraControls& controls, const ControllerSpecs& controllerSpec) :
			m_Up             ( camSpec.Up     ),
			m_Velocity       ( 0.0f           ),
			m_MousePos       ( 0.0f           ),
			m_MouseSpeed     ( 5.0f           ),
			m_Camera         ( camSpec        ),
			m_CameraControls ( controls       ),
			m_ControllerSpec ( controllerSpec )
	{}

	void OnUpdate();

	inline const auto& GetCamera()   const noexcept { return m_Camera;               }
	inline const auto  GetPosition() const noexcept { return m_Camera.GetPosition(); }

	inline void OnWindowResized(uint16_t width, uint16_t height) { m_Camera.SetAspectRatio(float(width) / float(height)); }

private:
	void CalculateVelocity();
	void CalcCameraOrientation();
	glm::vec3 CalculateAcceleration() const;

private:
	glm::vec3 m_Up;
	glm::vec3 m_Velocity;
	glm::vec2 m_MousePos;
	float     m_MouseSpeed;

	::PerspectiveCamera m_Camera;
	CameraControls      m_CameraControls;
	ControllerSpecs     m_ControllerSpec;
};
