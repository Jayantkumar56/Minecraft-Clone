

#pragma once

#include "Quirk.h"
#include "TitleBar.h"
#include "GamePanel.h"


class GameFrame : public Quirk::Frame {
public:
	GameFrame(Quirk::WindowSpecification& spec) :
			Frame(spec)
	{
		CustomRenderer::InitRenderer();

		SetTitleBar<GameTitleBar>();
		AddPanel<GamePanel>();
	}

	~GameFrame() = default;

	virtual void OnUpdate()			          override;
	virtual void OnImguiUiUpdate()            override;
	virtual bool OnEvent(Quirk::Event& event) override;

private:
	std::filesystem::path m_WorkingDirectory;
};
