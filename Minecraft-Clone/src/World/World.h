

#pragma once

#include "Player/Player.h"
#include "World/Chunk/Chunk.h"
#include "Renderer/Renderer.h"


class GamePanel;

class World {
	friend class GamePanel;
public:
	World() {
		m_Chunks.emplace_back();
		glm::vec3 Translation { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation    { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale       { 0.02f, 0.02f, 0.02f };

		glm::mat4 rotation  = glm::toMat4(glm::quat(Rotation));
		m_ChunksModelMatrix = glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}

	void OnUpdate()                   { m_Player.OnUpdate();            }
	bool OnEvent(Quirk::Event& event) { return m_Player.OnEvent(event); }

private:
	void HandleResize(uint16_t width, uint16_t height) {
		m_Player.m_CameraController.OnWindowResized(width, height);
	}

	void RenderWorld() {
		auto& camera = m_Player.m_CameraController;
		CustomRenderer::BeginScene(camera.GetProjectionViewMatrix(), camera.GetPosition());

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
};
