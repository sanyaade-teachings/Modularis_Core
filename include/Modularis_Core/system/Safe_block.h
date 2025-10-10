#pragma once

#include <stddef.h>

struct MDLRS_Safe_block
{
	struct MDLRS_Safe_block *previous, *next;
	const char *name;
	size_t size, count;
};