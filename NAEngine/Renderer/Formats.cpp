#include "Formats.h"

#include <map>
#include <string>

#include "Base/Util/Util.h"

namespace na
{
	static size_t FormatByteSizes[] = {
		   4, // R32_FLOAT,
		   8, // R32G32_FLOAT,
		   12, // R32G32B32_FLOAT,
		   16 // R32G32B32A32_FLOAT
	};

	static std::map<std::string, Format> FormatStringConversions = {
		{ "float", Format::R32_FLOAT },
		{ "float2", Format::R32G32_FLOAT },
		{ "float3", Format::R32G32B32_FLOAT },
		{ "float4", Format::R32G32B32A32_FLOAT }
	};

	static DXGI_FORMAT DXGIFormats[] = {
		   DXGI_FORMAT_R32_FLOAT, // R32_FLOAT,
		   DXGI_FORMAT_R32G32_FLOAT, // R32G32_FLOAT,
		   DXGI_FORMAT_R32G32B32_FLOAT, // R32G32B32_FLOAT,
		   DXGI_FORMAT_R32G32B32A32_FLOAT // R32G32B32A32_FLOAT
	};



	Format GetFormatFromString(const char *s)
	{
		return FormatStringConversions[s];
	}

	size_t GetFormatByteSize(Format format)
	{
		int numFormats = STATIC_ARRAY_SIZE(FormatByteSizes);
		NA_ASSERT_RETURN_VALUE((int)format < numFormats, 0, "Failed to find byte length for format %d", (int)format);

		return FormatByteSizes[(int)format];
	}

	DXGI_FORMAT GetDXGIFormat(Format format)
	{
		return DXGIFormats[(int)format];
	}
}