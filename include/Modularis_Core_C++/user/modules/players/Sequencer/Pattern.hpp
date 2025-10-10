#pragma once

namespace MDLRS
{
	struct Sequence;

	struct Pattern
	{
		float length;
		Sequence *tracks;
		unsigned track_count;
	};
}