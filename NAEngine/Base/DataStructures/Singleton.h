#pragma once

#define NA_CREATE_SINGLETON_INSTANCE(T) T* T::msInst = nullptr

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