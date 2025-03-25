

#pragma once


#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct PerspectiveCameraSpecs {
	float FOVY;
	float AspectRatio;         // (width / height)
	float ZNear;
	float ZFar;
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Forward;
};

class PerspectiveCamera {
public:
	PerspectiveCamera(const PerspectiveCameraSpecs& spec) :
			m_FOVY             ( spec.FOVY        ),
			m_AspectRatio      ( spec.AspectRatio ),
			m_ZNear            ( spec.ZNear       ),
			m_ZFar             ( spec.ZFar        ),
			m_Position         ( spec.Position    ),
			m_Orientation      ( glm::quatLookAt(spec.Forward, spec.Up) ),
			m_ViewMatrix       ( glm::mat4_cast(glm::conjugate(m_Orientation)) * glm::translate(glm::mat4(1.0f), -m_Position) ),
			m_ProjectionMatrix ( glm::perspective(spec.FOVY, spec.AspectRatio, spec.ZNear, spec.ZFar)                         ),
			m_ViewProjectionMatrix ( m_ProjectionMatrix * m_ViewMatrix )
	{}

	inline glm::mat4 GetViewMatrix()     const noexcept { return m_ViewMatrix;           }
	inline glm::mat4 GetViewProjection() const noexcept { return m_ViewProjectionMatrix; }
	inline glm::quat GetOrientation()    const noexcept { return m_Orientation;          }
	inline glm::vec3 GetPosition()       const noexcept { return m_Position;             }
	inline glm::vec3 GetRight()          const          { return glm::normalize(m_Orientation * glm::vec3(1, 0, 0));  }
	inline glm::vec3 GetForward()        const          { return glm::normalize(m_Orientation * glm::vec3(0, 0, -1)); }
	inline glm::vec3 GetUp()             const          { return glm::normalize(m_Orientation * glm::vec3(0, 1, 0));  }

	inline void SetOrientation(glm::quat orientation) { 
		m_Orientation = orientation; 
		UpdateViewAndViewProjection();
	}

	inline void AddMovementInput(const glm::vec3& offset) {
		m_Position += offset;
		UpdateViewAndViewProjection();
	}

	inline void AddPitchInput(float offset) {
		m_Orientation = glm::normalize(glm::angleAxis(glm::radians(-offset), GetRight()) * m_Orientation);
		UpdateViewAndViewProjection();
	}

	inline void AddYawInput(float offset) {
		m_Orientation = glm::normalize(glm::angleAxis(glm::radians(-offset), glm::vec3(0, 1, 0)) * m_Orientation);
		UpdateViewAndViewProjection();
	}

	inline void SetAspectRatio(float aspectRatio) {
		m_AspectRatio = aspectRatio;
		UpdateProjectionAndViewProjection(m_FOVY, m_AspectRatio, m_ZNear, m_ZFar);
	}

private:
	inline void UpdateViewAndViewProjection() {
		m_ViewMatrix           = glm::mat4_cast(glm::conjugate(m_Orientation)) * glm::translate(glm::mat4(1.0f), -m_Position);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	inline void UpdateProjectionAndViewProjection(float fovy, float aspectRatio, float zNear, float zFar) {
		m_ProjectionMatrix     = glm::perspective(fovy, aspectRatio, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

private:
	float m_FOVY;
	float m_AspectRatio;
	float m_ZNear;
	float m_ZFar;
	glm::vec3 m_Position;
	glm::quat m_Orientation;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewProjectionMatrix;
};
