#pragma once

#include <string>
#include <vector>

namespace na
{
#define CONSOLE_COMMAND(name)																								\
	static void __console_command_ ## name (const std::vector<ConsoleCommandParameter> &parameters);						\
	static ConsoleCommandRegistration __console_command_ ## name ## _registration(#name, __console_command_ ## name);		\
	static void __console_command_ ## name (const std::vector<ConsoleCommandParameter> &parameters)

#define CONSOLE_BOOL(name, varName, defaultValue)																			\
	static bool varName = defaultValue;																						\
	CONSOLE_COMMAND(name) {																									\
		if (parameters.size() > 0) { varName = parameters[0].AsBool(); }													\
		else { varName = !varName; }																						\
	}
	
	struct ConsoleCommandParameter
	{
		ConsoleCommandParameter(const std::string &value) : mValue(value) {}
		std::string mValue;

		bool AsBool()const;
		int AsInt()const;
		float AsFloat()const;
		std::string AsString()const;
	};

	typedef void(*ConsoleCommandFunc)(const std::vector<ConsoleCommandParameter> &parameters);

	class ConsoleCommandRegistration
	{
	public:
		ConsoleCommandRegistration(const std::string &name, ConsoleCommandFunc func);
	};

	bool DispatchConsoleCommand(const std::string &commandLine);

	void ConsolePrintf(const char *fmt, ...);


	void ConsoleSystemActivate(bool active);
	bool ConsoleSystemIsActive();

	void ConsoleSystemDebugRender();
}