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

#include <system/modules/Module.h>

#include <Modularis_Core/Modularis.h>
#include <stddef.h>
#include <system/safe memory.h>
#include <stdbool.h>

void MDLRS_Module_init(struct MDLRS_Module *self, struct MDLRS_Modularis *project)
{
	self->project=project;
	MDLRS_Port_group_init(&self->inputs, self);
	MDLRS_Port_group_init(&self->outputs, self);
	size_t count=safe_count(project->disconnected_modules);
	if (project->disconnected_module_count==count) project->disconnected_modules=safe_resize(project, project->disconnected_modules, 2*count);
	project->disconnected_modules[project->disconnected_module_count++]=self;
	self->index=project->disconnected_module_count;
	self->output_connections=0;
	self->ready=true;
}
struct MDLRS_Port_group *MDLRS_Module_get_inputs(struct MDLRS_Module *self)
{
	return &self->inputs;
}
struct MDLRS_Port_group *MDLRS_Module_get_outputs(struct MDLRS_Module *self)
{
	return &self->outputs;
}
struct MDLRS_Modularis *MDLRS_Module_get_project(struct MDLRS_Module *self)
{
	return self->project;
}
void MDLRS_Module_disconnect(struct MDLRS_Module *self)
{
	MDLRS_Port_group_disconnect_input(&self->inputs);
	MDLRS_Port_group_disconnect(&self->outputs);
}
void MDLRS_Module_deinit(struct MDLRS_Module *self)
{
	MDLRS_Port_group_deinit(&self->inputs);
	MDLRS_Port_group_deinit(&self->outputs);
	if (self->index)
	{
		struct MDLRS_Module **modules=self->project->disconnected_modules;
		unsigned count=self->project->disconnected_module_count--;
		for (unsigned a=self->index; a!=count; a++)
		{
			modules[a]->index--;
			modules[a-1]=modules[a];
		}
	}
}
void MDLRS_Module_dispose(struct MDLRS_Module *self)
{
	safe_dispose(self->project, self);
}