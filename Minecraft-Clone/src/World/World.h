

#pragma once

#include "Player/Player.h"
#include "World/Chunk/Chunk.h"
#include "Renderer/Renderer.h"
#include "Renderer/TextureManager.h"
#include "World/ChunkManager/ChunkManager.h"

class GamePanel;

class World {
	friend class GamePanel;
public:
	World() : m_ChunkManager() {
		m_TextureManager.LoadBlockSprites(m_BlockDataBase);

		//m_Chunks.emplace_back();
		m_ChunkManager.LoadChunksAroundPlayer(m_Player.GetPosition());
	}

	void OnUpdate() { 
		m_Player.OnUpdate();
		m_ChunkManager.OnUpdate(m_Player);
	}

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

	const Player& GetPlayer() { return m_Player; }

private:
	void HandleResize(uint16_t width, uint16_t height) {
		m_Player.m_CameraController.OnWindowResized(width, height);
	}

	void RenderWorld() {
		m_ChunkManager.RenderChunks(m_Player.GetCamera(), m_TextureManager);
	}

	glm::vec3 GetPlayerPosition() { return m_Player.GetPosition(); }

private:
	// player data;
	Player m_Player;

	TextureManager m_TextureManager;
	BlockDataBase  m_BlockDataBase;
	ChunkManager   m_ChunkManager;
};
