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

#include <system/modules/Output.h>

#include <stdbool.h>
#include <system/safe memory.h>
#include <ports/Sound.h>

static void on_update(void *self){}
static const struct MDLRS_IModule_f f=
{
	on_update
};

struct MDLRS_Output *MDLRS_Output_new(struct MDLRS_Modularis *project, unsigned channels)
{
	struct MDLRS_Output *result=safe_object(project, "MDLRS_Output", sizeof(struct MDLRS_Output));
	result->p.f=&f;

	result->p.project=project;
	MDLRS_Port_group_init(&result->p.inputs, &result->p);
	result->p.index=0;
	result->p.ready=true;
	result->channels=safe_array(project, "MDLRS_Sound @ MDLRS_Output_new()", sizeof(struct MDLRS_Sound), channels);
	for (unsigned a=0; a!=channels; a++)
	{
		MDLRS_Sound_init(result->channels+a, &result->p);
		MDLRS_Port_group_add(&result->p.inputs, &result->channels[a].p.p);
	}
	return result;
}
void MDLRS_Output_remove(struct MDLRS_Output *self)
{
	MDLRS_Port_group_disconnect_input(&self->p.inputs);
	MDLRS_Port_group_deinit(&self->p.inputs);
	unsigned count=safe_count(self->channels);
	for (unsigned a=0; a!=count; a++) MDLRS_Port_deinit(&self->channels[a].p);
	safe_dispose(self->p.project, self->channels);

	safe_dispose(self->p.project, self);
}