#pragma once

#include "NGADefs.h"

namespace na
{
	struct NGAInitParams
	{
	};

	void NGAInitialize(const NGAInitParams &params);
	void NGAShutdown();
}