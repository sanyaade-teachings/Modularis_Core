#pragma once

#include <cstddef>

namespace MDLRS
{
	struct Safe_block
	{
		Safe_block *previous, *next;
		const char *name;
		size_t size, count;
	};
}