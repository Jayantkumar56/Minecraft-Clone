

#include "Quirk.h"
#include "Core/Application/EntryPoint.h"

#include "GameLayer.h"

class MinecraftApp : public Quirk::Application {
public:
	MinecraftApp() : Application(L"Minecraft Clone") {
		Quirk::ImguiUI::Init();

		PushLayer(&m_GameLayer);
	}

	~MinecraftApp() {
		Quirk::ImguiUI::Terminate();
	}

private:
	GameLayer m_GameLayer;
};

namespace Quirk {
	
	void LaunchApp() {
		MinecraftApp app;
		app.Run();
	}

}

