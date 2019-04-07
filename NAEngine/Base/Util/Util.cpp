#include "Util.h"

namespace na
{
	int RoundToNearestMultiple(int num, int multiple)
	{
		if (multiple == 0)
			return num;

		int remainder = num % multiple;
		if (remainder == 0)
			return num;

		return num + multiple - remainder;
	}
}