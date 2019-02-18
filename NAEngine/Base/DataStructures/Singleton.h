#pragma once

namespace na
{
	template <typename T>
	class Singleton
	{
	public:
		static T* Get()
		{
			if (msInst == nullptr)
			{
				msInst = new T();
			}

			return msInst;
		}

	private:
		static T *msInst;
	};
}