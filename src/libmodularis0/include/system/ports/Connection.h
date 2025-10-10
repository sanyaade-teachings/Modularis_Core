#pragma once

struct MDLRS_Port;

struct Connection
{
	struct MDLRS_Port *port;
	unsigned index;
};