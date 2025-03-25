

#include "CameraController.h"

void CameraController::OnUpdate() {
	CalculateVelocity();

	float dt = static_cast<float>(Quirk::Time::GetDeltaTime());
	glm::vec3 posOffset = m_Velocity * dt;

	if (posOffset != glm::vec3(0.0f)) {
		m_Camera.AddMovementInput(posOffset);
	}

	if (Quirk::Input::IsKeyPressed(QK_Key_LeftMouseBtn))
		CalcCameraOrientation();

	m_MousePos.x = Quirk::Input::MouseCurrentX();
	m_MousePos.y = Quirk::Input::MouseCurrentY();
}

void CameraController::CalcCameraOrientation() {
	glm::vec2 mousePos   ( Quirk::Input::MouseCurrentX(),  Quirk::Input::MouseCurrentY()  );
	glm::vec2 deltaMouse ( mousePos - m_MousePos );

	deltaMouse *= m_MouseSpeed * Quirk::Time::GetDeltaTime();

	m_Camera.AddYawInput(deltaMouse.x);
	m_Camera.AddPitchInput(deltaMouse.y);
}

void CameraController::CalculateVelocity() {
	glm::vec3 acceleration = CalculateAcceleration();

	float dt = static_cast<float>(Quirk::Time::GetDeltaTime());

	if (acceleration == glm::vec3(0.0f)) {
		m_Velocity -= m_Velocity * std::min(dt * m_ControllerSpec.Deceleration, 1.0f);
	}
	else {
		m_Velocity += acceleration * m_ControllerSpec.Acceleration * dt;

		if (glm::length(m_Velocity) > m_ControllerSpec.MaxSpeed)
			m_Velocity = glm::normalize(m_Velocity) * m_ControllerSpec.MaxSpeed;
	}
}

glm::vec3 CameraController::CalculateAcceleration() const {
	glm::vec3 acceleration(0.0f);

	glm::vec3 right   = m_Camera.GetRight();
	glm::vec3 up      = m_Up;
	glm::vec3 forward = glm::cross(up, right);

	if (Quirk::Input::IsKeyPressed(m_CameraControls.Forward)) {
		acceleration += forward;
	}
	if (Quirk::Input::IsKeyPressed(m_CameraControls.Backward)) {
		acceleration += -forward;
	}
	if (Quirk::Input::IsKeyPressed(m_CameraControls.Right)) {
		acceleration += right;
	}
	if (Quirk::Input::IsKeyPressed(m_CameraControls.Left)) {
		acceleration += -right;
	}
	if (Quirk::Input::IsKeyPressed(m_CameraControls.Up)) {
		acceleration += up;
	}
	if (Quirk::Input::IsKeyPressed(m_CameraControls.Down)) {
		acceleration -= up;
	}

	return acceleration;
}
