/*
(C) 2022-2025 Серый MLGamer. All freedoms preserved.
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

#include <Modularis_Core/system/interfaces/system/modules/Module.h>
#include <Modularis_Core/extensions/modules/IModule.h>

#include <ports/Port_group.h>
#include <stdbool.h>

struct MDLRS_Modularis;

struct MDLRS_Module
{
	const void *f;

	struct MDLRS_Port_group inputs, outputs;
	struct MDLRS_Modularis *project;
	unsigned index;
	unsigned output_connections;
	bool ready;
};
typedef MDLRS_IModule MDLRS_Module;
void MDLRS_Module_init(struct MDLRS_Module *self, struct MDLRS_Modularis *project);
inline void MDLRS_Module_update(struct MDLRS_Module *self)
{
	if (self->ready)
	{
		self->ready=false;
		MDLRS_Port_group_update(&self->inputs);
		((MDLRS_Module)self->f)->on_update(self);
	}
}
inline void MDLRS_Module_get_ready(struct MDLRS_Module *self)
{
	if (self->ready) return;
	self->ready=true;
	MDLRS_Port_group_get_ready(&self->inputs);
}
void MDLRS_Module_deinit(struct MDLRS_Module *self);