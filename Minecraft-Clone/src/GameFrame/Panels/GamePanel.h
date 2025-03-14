

#pragma once

#include "Quirk.h"
#include "World/World.h"

class GamePanel : public Quirk::Panel {
public:
	GamePanel() : 
			Panel         ( "Game Panel" ),
			m_PanelWidth  ( 800 ),
			m_PanelHeight ( 800 ),
			m_Frame       ( Quirk::FrameBuffer::Create({ m_PanelWidth, m_PanelHeight }) )
	{
		Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
		m_Frame->SetAttachments({
			{ Quirk::FrameBufferTextureType::RGBA_8,			 {.RGBA = {0.10156f, 0.17968f, 0.20703f, 1.0f} } },
			{ Quirk::FrameBufferTextureType::RED_INTEGER,        {.RedInteger = -1   }						     },
			{ Quirk::FrameBufferTextureType::DEPTH_24_STENCIL_8, {.DepthValue = 1.0f }						     }
		});
	}

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;

	virtual void OnUpdate()		              override;
	virtual void OnImguiUiUpdate()            override;
	virtual bool OnEvent(Quirk::Event& event) override;

private:
	void CheckAndHandleResize();
	void RenderViewport();

private:
	uint16_t m_PanelWidth, m_PanelHeight;
	Quirk::Ref<Quirk::FrameBuffer> m_Frame;
	World m_GameWorld;
};
