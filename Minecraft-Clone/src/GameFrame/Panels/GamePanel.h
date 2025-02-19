

#pragma once

#include "Quirk.h"

class GamePanel : public Quirk::Panel {
public:
	GamePanel() : Panel("Game Panel") { }

	virtual void SetImguiProperties() override;

	virtual void OnUpdate()		              override;
	virtual void OnImguiUiUpdate()            override;
	virtual bool OnEvent(Quirk::Event& event) override;
};
