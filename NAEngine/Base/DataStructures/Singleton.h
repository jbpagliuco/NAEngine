#pragma once

namespace na
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			msInst = static_cast<T*>(this);
		}

		static T* Get()
		{
			return msInst;
		}

	protected:
		static T *msInst;
	};
}