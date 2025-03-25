

#pragma once

#include "Player/Player.h"
#include "World/Chunk/Chunk.h"
#include "Renderer/Renderer.h"
#include "Renderer/TextureManager.h"

class GamePanel;

class World {
	friend class GamePanel;
public:
	World() {
		m_Chunks.emplace_back();

		glm::vec3 Scale { 0.5f, 0.5f, 0.5f };
		m_ChunksModelMatrix = glm::scale(glm::mat4(1.0f), Scale);
	}

	void OnUpdate()                   { m_Player.OnUpdate();            }
	bool OnEvent(Quirk::Event& event) {
		Quirk::EventDispatcher::HandleEvent<Quirk::KeyPressedEvent>([&](Quirk::KeyPressedEvent& event) -> bool {
			if (event.GetKeyCode() == QK_Key_T) {
				m_Player.SetControl(true);
			}
			if (event.GetKeyCode() == QK_Key_R) {
				m_Player.SetControl(false);
			}

			return false;
		});

		return false;
	}

private:
	void HandleResize(uint16_t width, uint16_t height) {
		m_Player.m_CameraController.OnWindowResized(width, height);
	}

	void RenderWorld() {
		const auto& camera = m_Player.GetCamera();
		CustomRenderer::BeginScene(camera.GetViewProjection(), camera.GetPosition());

		for (auto& chunk : m_Chunks) {
			for (auto& subchunk : chunk.m_SubChunks) {
				CustomRenderer::SubmitMesh(subchunk.m_Mesh, m_ChunksModelMatrix);
			}
		}

		CustomRenderer::EndScene();
	}

	glm::vec3 GetPlayerPosition() { return m_Player.GetPosition(); }

private:
	// player data;
	Player m_Player;
	
	// chunks data;
	std::vector<Chunk> m_Chunks;

	glm::mat4 m_ChunksModelMatrix;

	//TextureManager m_TextureManager;
};
