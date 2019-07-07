#include "Engine.h"

#include <chrono>
#include <vector>

#include "Base/BaseSystem.h"
#include "Base/Debug/Assert.h"
#include "Base/Debug/Log.h"
#include "Renderer/ImguiRenderer.h"
#include "Renderer/RenderingSystem.h"

#include "Core/Assets/MeshAsset.h"
#include "Core/Assets/MaterialAsset.h"
#include "Core/Assets/TextureAsset.h"
#include "Core/Core.h"
#include "Core/Components/GameComponent.h"
#include "Core/Input/Input.h"
#include "Core/World/World.h"

namespace na
{
	float Frametime = 0.0f;

	struct SystemRegistration
	{
		typedef bool(*InitFunc)();
		typedef void(*Func)();

		std::string mSystemName;

		InitFunc mSystemInit;
		Func mSystemShutdown;

		Func mDoFrame;
		Func mDoFrameLate;

		SystemRegistration() :
			mSystemName(""),
			mSystemInit(nullptr),
			mSystemShutdown(nullptr),
			mDoFrame(nullptr),
			mDoFrameLate(nullptr)
		{
		}
	};

	std::vector<SystemRegistration> SystemRegistry;

	////////////////////////////////////////////////////////////////

	static void CheckDebugRendererSwitch();

	////////////////////////////////////////////////////////////////
	
	static void RegisterSystems()
	{
		{
			SystemRegistration reg;
			reg.mSystemName = "Base System";
			reg.mSystemInit = BaseSystemInit;
			reg.mSystemShutdown = BaseSystemShutdown;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Rendering System";
			reg.mSystemInit = RenderingSystemInit;
			reg.mSystemShutdown = RenderingSystemShutdown;
			reg.mDoFrame = RenderingSystemDoFrame;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Input System";
			reg.mSystemInit = InputSystemInit;
			reg.mSystemShutdown = InputSystemShutdown;
			reg.mDoFrameLate = InputSystemDoFrameLate;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Component System";
			reg.mSystemShutdown = GameComponentShutdown;
			reg.mDoFrame = GameComponentDoFrame;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "World System";
			SystemRegistry.push_back(reg);
		}
		
		{
			SystemRegistration reg;
			reg.mSystemName = "Mesh System";
			reg.mSystemInit = MeshSystemInit;
			reg.mSystemShutdown = MeshSystemShutdown;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Material System";
			reg.mSystemInit = MaterialSystemInit;
			reg.mSystemShutdown = MaterialSystemShutdown;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Texture System";
			reg.mSystemInit = TextureAssetSystemInit;
			reg.mSystemShutdown = TextureAssetSystemShutdown;
			SystemRegistry.push_back(reg);
		}
	}

	bool InitializeEngine()
	{
		RegisterSystems();

		LogInfo(CORE_LOG_FILTER, "=============================================");
		for (auto &sys : SystemRegistry)
		{
			LogInfo(CORE_LOG_FILTER, "Initializing %s", sys.mSystemName.c_str());
			if (sys.mSystemInit != nullptr) {
				bool ret = sys.mSystemInit();
				NA_ASSERT_RETURN_VALUE(ret, false, "Failed to initialize system '%s'.", sys.mSystemName.c_str());
			}
		}
		LogInfo(CORE_LOG_FILTER, "System initialization complete.");
		LogInfo(CORE_LOG_FILTER, "=============================================");
		LogLineBreak();

		return true;
	}

	void ShutdownEngine()
	{
		// Manually shutdown the world first
		WorldSystemShutdown();

		LogLineBreak();
		LogInfo(CORE_LOG_FILTER, "=============================================");
		for (auto it = SystemRegistry.rbegin(); it != SystemRegistry.rend(); ++it) {
			auto &sys = *it;
			LogInfo(CORE_LOG_FILTER, "Shutting down %s", sys.mSystemName.c_str());

			if (sys.mSystemShutdown != nullptr) {
				sys.mSystemShutdown();
			}
		}
		LogInfo(CORE_LOG_FILTER, "System shutdown complete.");
		LogInfo(CORE_LOG_FILTER, "=============================================");
	}

	void DoFrame()
	{
		auto start = std::chrono::high_resolution_clock::now();

		CheckDebugRendererSwitch();

		for (auto &sys : SystemRegistry) {
			if (sys.mDoFrame != nullptr) {
				sys.mDoFrame();
			}
		}

		for (auto &sys : SystemRegistry) {
			if (sys.mDoFrameLate != nullptr) {
				sys.mDoFrameLate();
			}
		}

		auto diff = std::chrono::high_resolution_clock::now() - start;
		Frametime = std::chrono::duration_cast<std::chrono::duration<float>>(diff).count();
	}

	


	static void CheckDebugRendererSwitch()
	{
		if (IsSystemKeyPressed(VK_F11)) {
			const bool newFocus = !ImguiRendererGetFocus();
			ForceShowCursor(newFocus);
			ImguiRendererSetFocus(newFocus);
		}
	}
}