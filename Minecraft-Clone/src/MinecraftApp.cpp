
#include "Quirk.h"
#include "Core/Application/EntryPoint.h"


class MinecraftApp : public Quirk::Application {
public:
	MinecraftApp() : Application(L"Minecraft Clone") {
		Quirk::ImguiUI::Init();
	}

	~MinecraftApp() = default;
};



namespace Quirk {
	
	void LaunchApp() {
		MinecraftApp app;
		app.Run();
	}

}

