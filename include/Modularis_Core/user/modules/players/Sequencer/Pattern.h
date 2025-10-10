#pragma once

struct MDLRS_Sequence;

struct MDLRS_Pattern
{
	float length;
	struct MDLRS_Sequence *tracks;
	unsigned track_count;
};