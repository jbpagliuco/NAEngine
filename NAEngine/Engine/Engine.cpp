#include "Engine.h"

#include <chrono>
#include <vector>

#include "Vendor/imgui/imgui.h"

#include "Base/BaseSystem.h"
#include "Base/Console/Console.h"
#include "Base/Debug/Assert.h"
#include "Base/Debug/Log.h"
#include "Base/Util/Timer.h"
#include "Renderer/ImguiRenderer.h"
#include "Renderer/RenderingSystem.h"

#include "Engine/Assets/MeshAsset.h"
#include "Engine/Assets/MaterialAsset.h"
#include "Engine/Assets/TextureAsset.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/Input/Input.h"
#include "Engine/Physics/Physics.h"
#include "Engine/World/World.h"

namespace na
{
	struct SystemRegistration
	{
		typedef bool(*InitFunc)();
		typedef void(*Func)();

		std::string mSystemName;

		InitFunc mSystemInit;
		InitFunc mSystemInitLate;
		Func mSystemShutdown;
		Func mSystemShutdownLate;

		Func mDoFrame;
		Func mDoFrameLate;

		SystemRegistration() :
			mSystemName(""),
			mSystemInit(nullptr),
			mSystemInitLate(nullptr),
			mSystemShutdown(nullptr),
			mSystemShutdownLate(nullptr),
			mDoFrame(nullptr),
			mDoFrameLate(nullptr)
		{
		}
	};

	static std::vector<SystemRegistration> SystemRegistry;

	// Time stats
	static float FrameTime = 0.0f;
	static double ElapsedTime = 0.0f;
	static Timestamp LastFrameEnd;

#if defined(_NA_DEBUG)
	// Debug timing stats
	CONSOLE_BOOL(show_frametime, FrameTimeDebug, false);
	CONSOLE_BOOL(show_frametime_log, FrameTimeDebugLog, false);
#endif

	////////////////////////////////////////////////////////////////

	static void DebugRender();
	static void CheckDebugRendererSwitch();

	static void DebugRenderFrameTime();

	////////////////////////////////////////////////////////////////
	
	static void RegisterSystems()
	{
		{
			SystemRegistration reg;
			reg.mSystemName = "Base System";
			reg.mSystemInit = BaseSystemInit;
			reg.mSystemShutdown = BaseSystemShutdown;
			reg.mDoFrame = BaseSystemDoFrame;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Rendering System";
			reg.mSystemInit = RenderingSystemInit;
			reg.mSystemInitLate = RenderingSystemInitLate;
			reg.mSystemShutdown = RenderingSystemShutdown;
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
			reg.mSystemName = "Physics System";
			reg.mSystemInit = PhysicsSystemInit;
			reg.mSystemShutdown = PhysicsSystemShutdown;
			reg.mDoFrame = PhysicsSystemDoFrame;
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
			reg.mSystemShutdownLate = MeshSystemShutdown;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Material System";
			reg.mSystemInit = MaterialSystemInit;
			reg.mSystemShutdownLate = MaterialSystemShutdown;
			SystemRegistry.push_back(reg);
		}

		{
			SystemRegistration reg;
			reg.mSystemName = "Texture System";
			reg.mSystemInit = TextureAssetSystemInit;
			reg.mSystemShutdownLate = TextureAssetSystemShutdown;
			SystemRegistry.push_back(reg);
		}
	}

	bool InitializeEngine()
	{
		RegisterSystems();

		LogInfo(ENGINE_LOG_FILTER, "=============================================");
		for (auto &sys : SystemRegistry)
		{
			LogInfo(ENGINE_LOG_FILTER, "Initializing %s", sys.mSystemName.c_str());
			if (sys.mSystemInit != nullptr) {
				bool ret = sys.mSystemInit();
				NA_ASSERT_RETURN_VALUE(ret, false, "Failed to initialize system '%s'.", sys.mSystemName.c_str());
			}
		}

		for (auto &sys : SystemRegistry)
		{
			LogInfo(ENGINE_LOG_FILTER, "Initializing %s", sys.mSystemName.c_str());
			if (sys.mSystemInitLate != nullptr) {
				bool ret = sys.mSystemInitLate();
				NA_ASSERT_RETURN_VALUE(ret, false, "Failed to initialize system '%s'.", sys.mSystemName.c_str());
			}
		}
		LogInfo(ENGINE_LOG_FILTER, "System initialization complete.");
		LogInfo(ENGINE_LOG_FILTER, "=============================================");
		LogLineBreak();

		LastFrameEnd = Timestamp::Now();

		return true;
	}

	void ShutdownEngine()
	{
		// Manually shutdown the world first
		WorldSystemShutdown();

		LogLineBreak();
		LogInfo(ENGINE_LOG_FILTER, "=============================================");
		for (auto it = SystemRegistry.rbegin(); it != SystemRegistry.rend(); ++it) {
			auto &sys = *it;
			LogInfo(ENGINE_LOG_FILTER, "Shutting down %s", sys.mSystemName.c_str());

			if (sys.mSystemShutdown != nullptr) {
				sys.mSystemShutdown();
			}
		}
		LogInfo(ENGINE_LOG_FILTER, "System shutdown complete.");
		LogInfo(ENGINE_LOG_FILTER, "=============================================");
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

		RenderingSystemBeginFrame();
		RenderingSystemDoFrame();

		DebugRender();

		RenderingSystemEndFrame();

		// Update frametime stats
		FrameTime = (Timestamp::Now() - LastFrameEnd).AsSeconds();

#if defined(_NA_DEBUG)
		if (FrameTimeDebugLog) {
			const uint64_t newElapsedTime = (uint64_t)(ElapsedTime + FrameTime);
			if (newElapsedTime - (uint64_t)ElapsedTime >= 1) {
				LogInfo(ENGINE_LOG_FILTER, "Frame Time: %.5f", FrameTime);
			}
		}
#endif

		ElapsedTime += FrameTime;
		LastFrameEnd = Timestamp::Now();
	}

	float GetEngineDeltaFrameTime()
	{
		return FrameTime;
	}

	double GetEngineElapsedTime()
	{
		return ElapsedTime;
	}
	

	static void DebugRender()
	{
		BaseSystemDebugRender();

		DebugRenderFrameTime();
	}

	static void CheckDebugRendererSwitch()
	{
		// Toggle the debug renderer when F11 is pressed.
		if (IsSystemKeyPressed(VK_F11)) {
			const bool newFocus = !ImguiRendererGetFocus();
			ForceShowCursor(newFocus);
			ImguiRendererSetFocus(newFocus);
		}

		// Toggle the console when F1 is pressed.
		if (IsSystemKeyPressed(VK_F1)) {
			const bool newFocus = !ConsoleSystemIsActive();
			ConsoleSystemActivate(newFocus);
			ForceShowCursor(newFocus);
			ImguiRendererSetFocus(newFocus);
		}
	}


	static void DebugRenderFrameTime()
	{
#if defined(_NA_DEBUG)
		if (FrameTimeDebug) {
			if (ImGui::Begin("Frame Time Debug", &FrameTimeDebug)) {
				ImGui::Text("Frame Time: %.5f", FrameTime);
				ImGui::Text("Total Elapsed Time: %.1f", ElapsedTime);
			}

			ImGui::End();
		}
#endif
	}
}