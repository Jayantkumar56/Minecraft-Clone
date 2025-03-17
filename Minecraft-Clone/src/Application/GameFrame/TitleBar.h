
#pragma once

#include "Quirk.h"


class GameTitleBar : public Quirk::TitleBar{
public:
	GameTitleBar() {
		m_CloseIcon    = Quirk::Texture2D::Create("assets/Images/close.png");
		m_MinimiseIcon = Quirk::Texture2D::Create("assets/Images/minus.png");
		m_MaximiseIcon = Quirk::Texture2D::Create("assets/Images/maximize.png");
	}

	~GameTitleBar() = default;

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;

	virtual void OnImguiUiUpdate() override;
	virtual bool OnEvent(Quirk::Event& event) override { return false; }

private:
	Quirk::Ref<Quirk::Texture2D> m_CloseIcon;
	Quirk::Ref<Quirk::Texture2D> m_MinimiseIcon;
	Quirk::Ref<Quirk::Texture2D> m_MaximiseIcon;
};
