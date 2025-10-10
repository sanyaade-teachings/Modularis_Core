/*
(C) 2023-2025 Серый MLGamer. All freedoms preserved.
Дзен: <https://dzen.ru/seriy_mlgamer>
SoundCloud: <https://soundcloud.com/seriy_mlgamer>
YouTube: <https://www.youtube.com/@Seriy_MLGamer>
GitVerse: <https://gitverse.ru/Seriy_MLGamer>
E-mail: <Seriy-MLGamer@yandex.ru>

This file is part of Modularis Core.
Modularis Core is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Modularis Core is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Modularis Core. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <Modularis_Core/system/interfaces/system/ports/Port.h>
#include <Modularis_Core/extensions/ports/IPort.h>
#include <system/ports/Any_port.h>

#include <system/safe memory.h>
#include <system/modules/Module.h>

struct MDLRS_Port_group;
struct Connection;

struct MDLRS_Port
{
	struct MDLRS_Any_port p;

	const char *type;
	struct Connection *connections;
	unsigned connection_count;
};
typedef const struct MDLRS_Port_f *MDLRS_Port;
struct MDLRS_Port_f
{
	struct MDLRS_Any_port_f f1;
	struct MDLRS_IPort_f f2;
};
void MDLRS_Port_init(struct MDLRS_Port *self, const char *type, struct MDLRS_Module *module);
inline void MDLRS_Port_deinit(struct MDLRS_Port *self)
{
	safe_dispose(self->p.module->project, self->connections);
}