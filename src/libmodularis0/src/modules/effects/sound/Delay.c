/*
(C) 2024-2025 Серый MLGamer. All freedoms preserved.
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

#include <modules/effects/sound/Delay.h>

#include <system/safe memory.h>
#include <Modularis_Core/Modularis.h>
#include <Modularis_Core/system/types/Sound_value.h>
#include <string.h>

static void on_update(struct MDLRS_Delay *self);
static struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Delay *MDLRS_Delay_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Delay", sizeof(struct MDLRS_Delay));
}
struct MDLRS_Delay *MDLRS_Delay_new(struct MDLRS_Modularis *project, float delay)
{
	struct MDLRS_Delay *result=MDLRS_Delay_allocate(project);
	MDLRS_Delay_init(result, project, delay);
	return result;
}
void MDLRS_Delay_init(struct MDLRS_Delay *self, struct MDLRS_Modularis *project, float delay)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Sound_init(&self->input, &self->p);
	MDLRS_Real_controller_init(&self->delay, &self->p, delay);
	MDLRS_Sound_init(&self->output, &self->p);
	MDLRS_Port_group_add(&self->p.inputs, &self->input.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->delay.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
	unsigned buffer_size=delay*self->p.project->sample_rate;
	if (buffer_size)
	{
		self->buffer=safe_array(project, "MDLRS_Sound_value @ MDLRS_Delay_init()", sizeof(MDLRS_Sound_value), buffer_size);
		memset(self->buffer, 0, sizeof(MDLRS_Sound_value)*buffer_size);
	}
	else self->buffer=safe_array(project, "MDLRS_Sound_value @ MDLRS_Delay_init()", sizeof(MDLRS_Sound_value), 0);
	self->position=0;
}
void on_update(struct MDLRS_Delay *self)
{
	unsigned buffer_size=self->delay.value*self->p.project->sample_rate;
	if (buffer_size)
	{
		if (buffer_size>safe_count(self->buffer)) self->buffer=safe_resize(self->p.project, self->buffer, buffer_size);
		self->output.frame=self->buffer[self->position];
		self->buffer[self->position]=self->input.frame;
		self->position=(self->position+1)%buffer_size;
	}
}
void MDLRS_Delay_set_delay(struct MDLRS_Delay *self, float delay)
{
	self->delay.value=delay;
}
struct MDLRS_Sound *MDLRS_Delay_get_input(struct MDLRS_Delay *self)
{
	return &self->input;
}
struct MDLRS_Real_controller *MDLRS_Delay_get_delay(struct MDLRS_Delay *self)
{
	return &self->delay;
}
struct MDLRS_Sound *MDLRS_Delay_get_output(struct MDLRS_Delay *self)
{
	return &self->output;
}
void MDLRS_Delay_deinit(struct MDLRS_Delay *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->input.p);
	MDLRS_Port_deinit(&self->delay.p);
	MDLRS_Port_deinit(&self->output.p);
	safe_dispose(self->p.project, self->buffer);

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Delay_remove(struct MDLRS_Delay *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->input.p);
	MDLRS_Port_deinit(&self->delay.p);
	MDLRS_Port_deinit(&self->output.p);
	safe_dispose(self->p.project, self->buffer);

	MDLRS_Module_deinit(&self->p);
	safe_dispose(self->p.project, self);
}