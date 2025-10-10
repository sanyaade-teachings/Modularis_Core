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

#include <ports/Note.h>

#include <system/safe memory.h>
#include <Modularis_Core/system/ports/Note/Note_event.h>
#include <stddef.h>
#include <system/ports/Connection.h>
#include <string.h>
#include <Modularis_Core/Modularis.h>

static void on_update(struct MDLRS_Note *self);
static const struct MDLRS_Port_f f=
{
	(int (*)(void *, struct MDLRS_Any_port *))MDLRS_Port_connect,
	(int (*)(void *, struct MDLRS_Port *))MDLRS_Port_connect_port,
	(int (*)(void *, struct MDLRS_Port_group *))MDLRS_Port_connect_group,
	(int (*)(void *))MDLRS_Port_disconnect,
	(int (*)(void *, struct MDLRS_Any_port *))MDLRS_Port_disconnect_from_port,
	(int (*)(void *, struct MDLRS_Port *))MDLRS_Port_disconnect_port,
	(int (*)(void *, struct MDLRS_Port_group *))MDLRS_Port_disconnect_group,
	(int (*)(void *))MDLRS_Port_disconnect_input,
	(void (*)(void *))MDLRS_Port_update,
	(void (*)(void *))MDLRS_Port_get_ready,
	(void (*)(void *))on_update
};

struct MDLRS_Note *MDLRS_Note_allocate(struct MDLRS_Module *module)
{
	return safe_object(module->project, "MDLRS_Note", sizeof(struct MDLRS_Note));
}
struct MDLRS_Note *MDLRS_Note_new(struct MDLRS_Module *module)
{
	struct MDLRS_Note *result=MDLRS_Note_allocate(module);
	MDLRS_Port_init(&result->p, "Note", module);
	result->p.p.f=&f;

	result->events=safe_array(module->project, "MDLRS_Note_event @ MDLRS_Note_new()", sizeof(struct MDLRS_Note_event), 8);
	result->event_count=0;
	return result;
}
void MDLRS_Note_init(struct MDLRS_Note *self, struct MDLRS_Module *module)
{
	MDLRS_Port_init(&self->p, "Note", module);
	self->p.p.f=&f;

	self->events=safe_array(module->project, "MDLRS_Note_event @ MDLRS_Note_init()", sizeof(struct MDLRS_Note_event), 8);
	self->event_count=0;
}
void on_update(struct MDLRS_Note *self)
{
	struct Connection *connections=self->p.connections;
	unsigned connection_count=self->p.connection_count;
	unsigned count=0;
	for (unsigned a=0; a!=connection_count; a++) count+=((struct MDLRS_Note *)connections[a].port)->event_count;
	if (count)
	{
		self->event_count=count;
		if (count>safe_count(self->events)) self->events=safe_resize(self->p.p.module->project, self->events, count);
		struct MDLRS_Note_event *events=self->events;
		for (unsigned a=0; a!=connection_count; a++)
		{
			count=((struct MDLRS_Note *)connections[a].port)->event_count;
			memcpy(events, ((struct MDLRS_Note *)connections[a].port)->events, sizeof(struct MDLRS_Note_event)*count);
			events+=count;
		}
	}
	else self->event_count=0;
}
struct MDLRS_Note_event *MDLRS_Note_get_events(struct MDLRS_Note *self)
{
	return self->events;
}
unsigned MDLRS_Note_get_event_count(struct MDLRS_Note *self)
{
	return self->event_count;
}
unsigned MDLRS_Note_add_start(struct MDLRS_Note *self, float pitch, float velocity, float phase)
{
	struct MDLRS_Modularis *project=self->p.p.module->project;
	unsigned result=project->note_scancode++;
	size_t count=safe_count(self->events);
	if (self->event_count==count) self->events=safe_resize(project, self->events, 2*count);
	self->events[self->event_count++]=(struct MDLRS_Note_event){NOTE_START, result, pitch, velocity, phase};
	return result;
}
void MDLRS_Note_add_change(struct MDLRS_Note *self, unsigned scancode, float pitch, float velocity)
{
	size_t count=safe_count(self->events);
	if (self->event_count==count) self->events=safe_resize(self->p.p.module->project, self->events, 2*count);
	self->events[self->event_count++]=(struct MDLRS_Note_event){NOTE_CHANGE, scancode, pitch, velocity};
}
void MDLRS_Note_add_stop(struct MDLRS_Note *self, unsigned scancode)
{
	size_t count=safe_count(self->events);
	if (self->event_count==count) self->events=safe_resize(self->p.p.module->project, self->events, 2*count);
	self->events[self->event_count++]=(struct MDLRS_Note_event){NOTE_STOP, scancode};
}
void MDLRS_Note_clean(struct MDLRS_Note *self)
{
	self->event_count=0;
}
void MDLRS_Note_deinit(struct MDLRS_Note *self)
{
	safe_dispose(self->p.p.module->project, self->events);
	MDLRS_Port_deinit(&self->p);
}
void MDLRS_Note_remove(struct MDLRS_Note *self)
{
	struct MDLRS_Modularis *project=self->p.p.module->project;
	safe_dispose(project, self->events);
	MDLRS_Port_deinit(&self->p);
	safe_dispose(project, self);
}