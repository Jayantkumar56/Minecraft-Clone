


#include "Quirk.h"
#include "Core/Application/EntryPoint.h"

#include "Theme.h"
#include "GameFrame/GameFrame.h"

using namespace Quirk;

class MinecraftCloneApp : public Application {
public:
	MinecraftCloneApp() :
		Application(L"Minecraft Clone", RendererAPI::API::OpenGL)
	{
		LoadFonts();

		WindowSpecification tempSpec{ "Minecraft Clone", 1600, 900, 200, 50, true, false, true };
		AddFrame<GameFrame>(tempSpec);
		Theme::SetTheme(ThemeName::DarkTheme);
	}

	~MinecraftCloneApp() {

	}

private:
	void LoadFonts() {
		//FontManager::SetFontFileToFontWeight(FontWeight::Regular, std::string("assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf"));
		FontManager::SetFontFileToFontWeight(FontWeight::Regular,   "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf");
		FontManager::SetFontFileToFontWeight(FontWeight::Medium,    "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Medium.ttf");
		FontManager::SetFontFileToFontWeight(FontWeight::SemiBold,  "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-SemiBold.ttf");
		FontManager::SetFontFileToFontWeight(FontWeight::Bold,      "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Bold.ttf");
		FontManager::SetFontFileToFontWeight(FontWeight::ExtraBold, "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-ExtraBold.ttf");
		FontManager::SetFontFileToFontWeight(FontWeight::Black,     "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Black.ttf");

		// EditorFrame related fonts 
		FontManager::LoadFont(FontWeight::Regular,  22                     );
		FontManager::LoadFont(FontWeight::Bold,     18                     );
		FontManager::LoadFont(FontWeight::Regular,  16, "DragFloatValue"   );
		FontManager::LoadFont(FontWeight::SemiBold, 20, "PropertyLabel"    );
		FontManager::LoadFont(FontWeight::SemiBold, 22, "ComponentTreeNode");
	}
};

namespace Quirk {

	// LaunchApp() must be defined in the application
	// Create app object inside this function and call Run method
	void LaunchApp() {
		MinecraftCloneApp app;
		app.Run();
	}

}

