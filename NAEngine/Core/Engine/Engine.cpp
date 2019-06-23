#include "Engine.h"

#include <chrono>
#include <vector>

#include "Base/BaseSystem.h"
#include "Base/Debug/Assert.h"
#include "Base/Debug/Log.h"
#include "Renderer/RenderingSystem.h"

#include "Core/AssetLoaders/AssetLoaderSystem.h"
#include "Core/Core.h"
#include "Core/Components/GameComponent.h"
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

		SystemRegistration() :
			mSystemName(""),
			mSystemInit(nullptr),
			mSystemShutdown(nullptr),
			mDoFrame(nullptr)
		{
		}
	};

	std::vector<SystemRegistration> SystemRegistry;

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
			reg.mSystemName = "Asset Loader System";
			reg.mSystemInit = AssetLoaderSystemInitialize;
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
			reg.mSystemShutdown = WorldSystemShutdown;
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

		for (auto &sys : SystemRegistry)
		{
			if (sys.mDoFrame != nullptr) {
				sys.mDoFrame();
			}
		}

		auto diff = std::chrono::high_resolution_clock::now() - start;
		Frametime = std::chrono::duration_cast<std::chrono::duration<float>>(diff).count();
	}
}