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
				msInst = this;
			}

			return msInst;
		}

	private:
		T *msInst = nullptr;
	};
}