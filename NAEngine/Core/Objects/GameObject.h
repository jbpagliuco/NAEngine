#pragma once

namespace na
{
	class GameObject
	{
	public:
		const char* GetName()const;

	private:
		const char *mName;
	};
}