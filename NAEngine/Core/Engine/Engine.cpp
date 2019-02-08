#include "Engine.h"

#include <vector>

#include "Base/BaseSystem.h"
#include "Base/Debug/Log.h"

#include "Core/Core.h"

namespace na
{
	struct SystemRegistration
	{
		typedef bool(*InitFunc)();
		typedef void(*Func)();

		const char *mSystemName;

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
	}

	bool InitializeEngine()
	{
		RegisterSystems();

		LogInfo(CORE_LOG_FILTER, "=============================================");
		for (auto &sys : SystemRegistry)
		{
			LogInfo(CORE_LOG_FILTER, "Initializing %s", sys.mSystemName);
			if (sys.mSystemInit != nullptr && sys.mSystemInit() == false) {
				LogError(CORE_LOG_FILTER, "System initialized failed.");
				return false;
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
		for (auto &sys : SystemRegistry)
		{
			LogInfo(CORE_LOG_FILTER, "Shutting down %s", sys.mSystemName);

			if (sys.mSystemShutdown != nullptr) {
				sys.mSystemShutdown();
			}
		}
		LogInfo(CORE_LOG_FILTER, "System shutdown complete.");
		LogInfo(CORE_LOG_FILTER, "=============================================");
	}

	void DoFrame()
	{
		for (auto &sys : SystemRegistry)
		{
			if (sys.mDoFrame != nullptr) {
				sys.mDoFrame();
			}
		}
	}
}