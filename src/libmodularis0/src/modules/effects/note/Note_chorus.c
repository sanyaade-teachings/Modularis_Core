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

#include <modules/effects/note/Note_chorus.h>

#include <system/safe memory.h>
#include <stddef.h>
#include <stdint.h>
#include <Modularis_Core/system/ports/Note/Note_event.h>
#include <math.h>
#include <stdlib.h>

static void on_update(struct MDLRS_Note_chorus *self);
static struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Note_chorus *MDLRS_Note_chorus_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Note_chorus", sizeof(struct MDLRS_Note_chorus));
}
struct MDLRS_Note_chorus *MDLRS_Note_chorus_new(struct MDLRS_Modularis *project, float spread, unsigned voices)
{
	struct MDLRS_Note_chorus *result=MDLRS_Note_chorus_allocate(project);
	MDLRS_Note_chorus_init(result, project, spread, voices);
	return result;
}
void MDLRS_Note_chorus_init(struct MDLRS_Note_chorus *self, struct MDLRS_Modularis *project, float spread, unsigned voices)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Note_init(&self->input, &self->p);
	MDLRS_Real_controller_init(&self->spread, &self->p, spread);
	MDLRS_Integer_controller_init(&self->voices, &self->p, voices);
	MDLRS_Integer_controller_init(&self->random_phases, &self->p, 1);
	MDLRS_Note_init(&self->output, &self->p);
	MDLRS_Note_table_init(&self->notes, project);
	self->last=NULL;
	MDLRS_Port_group_add(&self->p.inputs, &self->input.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->spread.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->voices.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->random_phases.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
}
void on_update(struct MDLRS_Note_chorus *self)
{
	self->output.event_count=0;
	if (self->input.event_count)
	{
		struct MDLRS_Note_event *event;
		unsigned *chorus;
		int32_t voices;
		if (self->random_phases.value) for (unsigned a=0; a!=self->input.event_count; a++)
		{
			event=self->input.events+a;
			switch (event->type)
			{
				case NOTE_START:
					if (self->last)
					{
						chorus=(unsigned *)self->last;
						if (sizeof(unsigned)*self->voices.value>safe_count(chorus)-sizeof(struct Chorus_stack)) chorus=safe_resize(self->p.project, chorus, sizeof(struct Chorus_stack)+sizeof(unsigned)*self->voices.value);
						self->last=((struct Chorus_stack *)chorus)->previous;
					}
					else chorus=safe_array(self->p.project, "Chorus_stack @ on_update()", 1, sizeof(struct Chorus_stack)+sizeof(unsigned)*self->voices.value);
					voices=((struct Chorus_stack *)chorus)->voices=self->voices.value;
					chorus=(unsigned *)((struct Chorus_stack *)chorus+1);
					for (int32_t a=0; a!=voices; a++) chorus[a]=MDLRS_Note_add_start(&self->output, powf(2, self->spread.value*(2*a+1-voices)/(24*(voices-1)))*event->pitch, event->velocity, rand()/(float)((uint64_t)RAND_MAX+1));
					MDLRS_Note_table_set(&self->notes, event->scancode, chorus);
					break;
				case NOTE_CHANGE:
					chorus=MDLRS_Note_table_get(&self->notes, event->scancode);
					voices=((struct Chorus_stack *)chorus-1)->voices;
					for (int32_t a=0; a!=voices; a++) MDLRS_Note_add_change(&self->output, chorus[a], powf(2, self->spread.value*(2*a+1-self->voices.value)/(24*(self->voices.value-1)))*event->pitch, event->velocity);
					break;
				case NOTE_STOP:
					chorus=MDLRS_Note_table_get(&self->notes, event->scancode);
					voices=((struct Chorus_stack *)chorus-1)->voices;
					for (int32_t a=0; a!=voices; a++) MDLRS_Note_add_stop(&self->output, chorus[a]);
					MDLRS_Note_table_unset(&self->notes, event->scancode);
					chorus=(unsigned *)((struct Chorus_stack *)chorus-1);
					((struct Chorus_stack *)chorus)->previous=self->last;
					self->last=(struct Chorus_stack *)chorus;
			}
		}
		else for (unsigned a=0; a!=self->input.event_count; a++)
		{
			event=self->input.events+a;
			switch (event->type)
			{
				case NOTE_START:
					if (self->last)
					{
						chorus=(unsigned *)self->last;
						if (sizeof(unsigned)*self->voices.value>safe_count(chorus)-sizeof(struct Chorus_stack)) chorus=safe_resize(self->p.project, chorus, sizeof(struct Chorus_stack)+sizeof(unsigned)*self->voices.value);
						self->last=((struct Chorus_stack *)chorus)->previous;
					}
					else chorus=safe_array(self->p.project, "Chorus_stack @ on_update()", 1, sizeof(struct Chorus_stack)+sizeof(unsigned)*self->voices.value);
					voices=((struct Chorus_stack *)chorus)->voices=self->voices.value;
					chorus=(unsigned *)((struct Chorus_stack *)chorus+1);
					for (int32_t a=0; a!=voices; a++) chorus[a]=MDLRS_Note_add_start(&self->output, powf(2, self->spread.value*(2*a+1-voices)/(24*(voices-1)))*event->pitch, event->velocity, event->phase);
					MDLRS_Note_table_set(&self->notes, event->scancode, chorus);
					break;
				case NOTE_CHANGE:
					chorus=MDLRS_Note_table_get(&self->notes, event->scancode);
					voices=((struct Chorus_stack *)chorus-1)->voices;
					for (int32_t a=0; a!=voices; a++) MDLRS_Note_add_change(&self->output, chorus[a], powf(2, self->spread.value*(2*a+1-self->voices.value)/(24*(self->voices.value-1)))*event->pitch, event->velocity);
					break;
				case NOTE_STOP:
					chorus=MDLRS_Note_table_get(&self->notes, event->scancode);
					voices=((struct Chorus_stack *)chorus-1)->voices;
					for (int32_t a=0; a!=voices; a++) MDLRS_Note_add_stop(&self->output, chorus[a]);
					MDLRS_Note_table_unset(&self->notes, event->scancode);
					chorus=(unsigned *)((struct Chorus_stack *)chorus-1);
					((struct Chorus_stack *)chorus)->previous=self->last;
					self->last=(struct Chorus_stack *)chorus;
			}
		}
	}
}
void MDLRS_Note_chorus_set_spread(struct MDLRS_Note_chorus *self, float spread)
{
	self->spread.value=spread;
}
void MDLRS_Note_chorus_set_voices(struct MDLRS_Note_chorus *self, unsigned voices)
{
	self->voices.value=voices;
}
void MDLRS_Note_chorus_set_random_phases(struct MDLRS_Note_chorus *self, bool random_phases)
{
	self->random_phases.value=random_phases;
}
struct MDLRS_Note *MDLRS_Note_chorus_get_input(struct MDLRS_Note_chorus *self)
{
	return &self->input;
}
struct MDLRS_Real_controller *MDLRS_Note_chorus_get_spread(struct MDLRS_Note_chorus *self)
{
	return &self->spread;
}
struct MDLRS_Integer_controller *MDLRS_Note_chorus_get_voices(struct MDLRS_Note_chorus *self)
{
	return &self->voices;
}
struct MDLRS_Integer_controller *MDLRS_Note_chorus_get_random_phases(struct MDLRS_Note_chorus *self)
{
	return &self->random_phases;
}
struct MDLRS_Note *MDLRS_Note_chorus_get_output(struct MDLRS_Note_chorus *self)
{
	return &self->output;
}
void MDLRS_Note_chorus_deinit(struct MDLRS_Note_chorus *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Note_deinit(&self->input);
	MDLRS_Port_deinit(&self->spread.p);
	MDLRS_Port_deinit(&self->voices.p);
	MDLRS_Port_deinit(&self->random_phases.p);
	MDLRS_Note_deinit(&self->output);
	unsigned size=safe_count(self->notes.elements);
	for (unsigned a=0; a!=size; a++) if (self->notes.elements[a].exists) safe_dispose(self->p.project, self->notes.elements[a].data-sizeof(struct Chorus_stack));
	MDLRS_Note_table_deinit(&self->notes);
	struct Chorus_stack *previous;
	struct Chorus_stack *a=self->last; while (a)
	{
		previous=a->previous;
		safe_dispose(self->p.project, a);
		a=previous;
	}

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Note_chorus_remove(struct MDLRS_Note_chorus *self)
{
	MDLRS_Note_chorus_deinit(self);
	safe_dispose(self->p.project, self);
}