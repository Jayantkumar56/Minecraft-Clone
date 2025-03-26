


#include "Player.h"

Player::Player() :
		m_CameraController(
			PerspectiveCameraSpecs {
				.FOVY        { glm::radians(45.0f)          },
				.AspectRatio { 1.0f                         },
				.ZNear       { 1.0f                         },
				.ZFar        { 1000.0f                      },
				.Position    { glm::vec3(0.0f, 9.0f, 0.0f)  },
				.Up          { glm::vec3(0.0f, 1.0f, 0.0f)  },
				.Forward     { glm::vec3(0.0f, 0.0f, 1.0f) },
			},
			CameraControls {
				.Forward { QK_Key_W     }, .Backward { QK_Key_S        },
				.Right   { QK_Key_D     }, .Left     { QK_Key_A        },
				.Up      { QK_Key_Space }, .Down     { QK_Key_LeftCtrl }
			},
			ControllerSpecs {
				.MaxSpeed        { 25.0f  },
				.Acceleration { 10.0f },
				.Deceleration { 15.0f }
			}
		)
{
}
