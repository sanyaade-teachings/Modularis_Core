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

#include <modules/effects/note/Transpose.h>

#include <system/safe memory.h>
#include <Modularis_Core/system/ports/Note/Note_event.h>
#include <stddef.h>
#include <math.h>

static void on_update(struct MDLRS_Transpose *self);
static struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Transpose *MDLRS_Transpose_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Transpose", sizeof(struct MDLRS_Transpose));
}
struct MDLRS_Transpose *MDLRS_Transpose_new(struct MDLRS_Modularis *project, float transposition)
{
	struct MDLRS_Transpose *result=MDLRS_Transpose_allocate(project);
	MDLRS_Transpose_init(result, project, transposition);
	return result;
}
void MDLRS_Transpose_init(struct MDLRS_Transpose *self, struct MDLRS_Modularis *project, float transposition)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Note_init(&self->input, &self->p);
	MDLRS_Real_controller_init(&self->transposition, &self->p, transposition);
	MDLRS_Real_controller_init(&self->velocity, &self->p, 1);
	MDLRS_Note_init(&self->output, &self->p);
	MDLRS_Note_table_init(&self->notes, project);
	MDLRS_Port_group_add(&self->p.inputs, &self->input.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->transposition.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->velocity.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
}
void on_update(struct MDLRS_Transpose *self)
{
	self->output.event_count=0;
	for (unsigned a=0; a!=self->input.event_count; a++)
	{
		struct MDLRS_Note_event *event=self->input.events+a;
		switch (event->type)
		{
			case NOTE_START:
				MDLRS_Note_table_set(&self->notes, event->scancode, (void *)(size_t)MDLRS_Note_add_start(&self->output, powf(2, self->transposition.value/12)*event->pitch, self->velocity.value*event->velocity, event->phase));
				break;
			case NOTE_CHANGE:
				MDLRS_Note_add_change(&self->output, (size_t)MDLRS_Note_table_get(&self->notes, event->scancode), powf(2, self->transposition.value/12)*event->pitch, self->velocity.value*event->velocity);
				break;
			case NOTE_STOP:
				MDLRS_Note_add_stop(&self->output, (size_t)MDLRS_Note_table_get(&self->notes, event->scancode));
				MDLRS_Note_table_unset(&self->notes, event->scancode);
		}
	}
}
void MDLRS_Transpose_set_transposition(struct MDLRS_Transpose *self, float transposition)
{
	self->transposition.value=transposition;
}
void MDLRS_Transpose_set_velocity(struct MDLRS_Transpose *self, float velocity)
{
	self->velocity.value=velocity;
}
struct MDLRS_Note *MDLRS_Transpose_get_input(struct MDLRS_Transpose *self)
{
	return &self->input;
}
struct MDLRS_Real_controller *MDLRS_Transpose_get_transposition(struct MDLRS_Transpose *self)
{
	return &self->transposition;
}
struct MDLRS_Real_controller *MDLRS_Transpose_get_velocity(struct MDLRS_Transpose *self)
{
	return &self->velocity;
}
struct MDLRS_Note *MDLRS_Transpose_get_output(struct MDLRS_Transpose *self)
{
	return &self->output;
}
void MDLRS_Transpose_deinit(struct MDLRS_Transpose *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Note_deinit(&self->input);
	MDLRS_Real_controller_deinit(&self->transposition);
	MDLRS_Real_controller_deinit(&self->velocity);
	MDLRS_Note_deinit(&self->output);
	MDLRS_Note_table_deinit(&self->notes);

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Transpose_remove(struct MDLRS_Transpose *self)
{
	MDLRS_Transpose_deinit(self);
	safe_dispose(self->p.project, self);
}