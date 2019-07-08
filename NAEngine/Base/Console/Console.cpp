#include "Console.h"

#include <map>
#include <sstream>

#include "Debug/Assert.h"
#include "Util/Util.h"
#include "Vendor/imgui/imgui.h"

namespace na
{
#define CONSOLE_COMMANDS ConsoleCommands()

	constexpr size_t MAX_CONSOLE_COMMANDS = 100;
	constexpr size_t MAX_OUTPUT_GROUPS = 5;

	struct InternalCommand
	{
		InternalCommand() : mName(""), mFunc(nullptr) {}
		InternalCommand(const std::string &name, ConsoleCommandFunc func) : mName(name), mFunc(func) {}

		std::string mName;
		ConsoleCommandFunc mFunc;
	};
	static size_t NumCommands = 0;

	static bool ConsoleDebug = false;

	static std::vector<std::string> ConsoleOutput[MAX_OUTPUT_GROUPS];
	static int ConsoleOutputStart = 0;
	static int NumConsoleOutputGroups = 0;

	static std::vector<std::string> ConsoleHistory;
	static int ConsoleHistoryIndex = -1;
	

	static InternalCommand* ConsoleCommands()
	{
		static InternalCommand _Commands[MAX_CONSOLE_COMMANDS];
		return _Commands;
	}

	static ConsoleCommandFunc GetCommandByName(const std::string &name)
	{
		for (int i = 0; i < NumCommands; ++i) {
			if (CONSOLE_COMMANDS[i].mName == name) {
				return CONSOLE_COMMANDS[i].mFunc;
			}
		}

		return nullptr;
	}


	static void StartNewGroup()
	{
		++ConsoleOutputStart;
		if (ConsoleOutputStart >= MAX_OUTPUT_GROUPS) {
			ConsoleOutputStart = 0;
		}

		ConsoleOutput[ConsoleOutputStart] = std::vector<std::string>();
	}
	
	static void AddConsoleOutput(const std::string &line)
	{
		ConsoleOutput[ConsoleOutputStart].push_back(line);

		NumConsoleOutputGroups = Min(NumConsoleOutputGroups + 1, (int)MAX_OUTPUT_GROUPS);
	}

	

	bool ConsoleCommandParameter::AsBool()const
	{
		return mValue == "true";
	}

	int ConsoleCommandParameter::AsInt()const
	{
		return std::stoi(mValue);
	}

	float ConsoleCommandParameter::AsFloat()const
	{
		return std::stof(mValue);
	}

	std::string ConsoleCommandParameter::AsString()const
	{
		return mValue;
	}


	ConsoleCommandRegistration::ConsoleCommandRegistration(const std::string &name, ConsoleCommandFunc func)
	{
		NA_ASSERT_RETURN(NumCommands < MAX_CONSOLE_COMMANDS, "Need to bump max number of console commands.");

		CONSOLE_COMMANDS[NumCommands] = InternalCommand(name, func);
		++NumCommands;
	}


	bool DispatchConsoleCommand(const std::string &commandLine)
	{
		std::stringstream ss(commandLine);

		std::string command;
		ss >> command;

		// Does this console command exist?
		ConsoleCommandFunc f = GetCommandByName(command);
		if (f == nullptr) {
			return false;
		}

		std::vector<ConsoleCommandParameter> params;
		for (std::string s; ss >> s;) {
			params.push_back(s);
		}

		f(params);

		// The command was executed.
		return true;
	}



	void ConsolePrintf(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		{
			char message[1024];
			vsprintf_s(message, fmt, args);

			AddConsoleOutput(message);
		}
		va_end(args);

	}


	void ConsoleSystemActivate(bool active)
	{
		ConsoleDebug = active;
	}

	bool ConsoleSystemIsActive()
	{
		return ConsoleDebug;
	}


	void ConsoleSystemDebugRender()
	{
		if (!ConsoleDebug) {
			return;
		}

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(800.0f, -1.0f));

		if (ImGui::Begin("Console System", &ConsoleDebug)) {
			static char commandLine[1024] = { 0 };

			auto historyCallback = [](ImGuiInputTextCallbackData *pData) {
				if (ConsoleHistoryIndex == -1) {
					ConsoleHistoryIndex = (int)ConsoleHistory.size();
				}

				bool writeHistory = false;
				if (pData->EventKey == ImGuiKey_UpArrow) {
					ConsoleHistoryIndex = Max(0, ConsoleHistoryIndex - 1);
					writeHistory = true;
				} else if (pData->EventKey == ImGuiKey_DownArrow) {
					ConsoleHistoryIndex = Min((int)ConsoleHistory.size(), ConsoleHistoryIndex + 1);
					writeHistory = true;
				}

				if (writeHistory) {
					char buf[1024];
					if (ConsoleHistoryIndex < ConsoleHistory.size()) {
						strncpy_s(buf, 1024, ConsoleHistory[ConsoleHistoryIndex].c_str(), 1024);
					} else {
						buf[0] = 0;
					}

					strncpy_s(pData->Buf, pData->BufSize - 1, buf, pData->BufSize - 1);
					pData->BufDirty = true;
					pData->BufTextLen = (int)strlen(pData->Buf);
					pData->CursorPos = pData->BufTextLen;
				}

				return 0;
			};

			ImGui::SetKeyboardFocusHere();
			if (ImGui::InputText("Command Line", commandLine, 1024, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, historyCallback)) {
				if (commandLine[0] != 0) {
					ConsoleHistory.push_back(commandLine);
					ConsoleHistoryIndex = -1;

					if (DispatchConsoleCommand(commandLine)) {
						AddConsoleOutput("---- " + std::string(commandLine));
					} else {
						AddConsoleOutput("---- '" + std::string(commandLine) + "' command not found!");
					}

					commandLine[0] = 0;
					StartNewGroup();
				}
			}

			ImGui::NewLine();
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "Console Ouptut:");
			
			for (int i = ConsoleOutputStart - 1; i >= 0; --i) {
				for (auto &it : ConsoleOutput[i]) {
					ImGui::Text("%s", it.c_str());
				}
			}

			for (int i = NumConsoleOutputGroups - 1; i >= ConsoleOutputStart; --i) {
				for (auto &it : ConsoleOutput[i]) {
					ImGui::Text("%s", it.c_str());
				}
			}
		}

		ImGui::End();
	}
}