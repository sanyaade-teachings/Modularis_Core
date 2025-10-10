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

#include <modules/synthesizers/Oscillator.h>

#include <stdint.h>
#include <system/safe memory.h>
#include <stddef.h>
#include <Modularis_Core/Modularis.h>
#include <math.h>
#include <Modularis_Core/system/ports/Note/Note_event.h>

static void on_update(struct MDLRS_Oscillator *self);
static struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Oscillator *MDLRS_Oscillator_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Oscillator", sizeof(struct MDLRS_Oscillator));
}
struct MDLRS_Oscillator *MDLRS_Oscillator_new(struct MDLRS_Modularis *project)
{
	struct MDLRS_Oscillator *result=MDLRS_Oscillator_allocate(project);
	MDLRS_Oscillator_init(result, project);
	return result;
}
void MDLRS_Oscillator_init(struct MDLRS_Oscillator *self, struct MDLRS_Modularis *project)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Note_init(&self->input, &self->p);
	MDLRS_Real_controller_init(&self->volume, &self->p, 1);
	MDLRS_Integer_controller_init(&self->waveform, &self->p, 0);
	MDLRS_ADSR_init(&self->envelope, &self->p, 0, 0, 1, 0);
	MDLRS_Sound_init(&self->output, &self->p);
	MDLRS_Note_table_init(&self->pressed_table, project);
	self->pressed=NULL;
	self->released=NULL;
	self->unused=NULL;
	MDLRS_Port_group_add(&self->p.inputs, &self->input.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->volume.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->waveform.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->envelope.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
}
static void generate(struct MDLRS_Oscillator *self, float (*waveform)(float phase))
{
	struct Oscillation *oscillation;
	self->output.frame=0;
	unsigned sample_rate=self->p.project->sample_rate;
	for (oscillation=self->pressed; oscillation; oscillation=oscillation->previous)
	{
		switch (oscillation->state)
		{
			case ATTACK:
				if (oscillation->time!=(uint32_t)(sample_rate*self->envelope.attack.value)) break;
				oscillation->state=DECAY;
				oscillation->time=0;
			case DECAY:
				if (oscillation->time!=(uint32_t)(sample_rate*self->envelope.decay.value)) break;
				oscillation->state=SUSTAIN;
		}
		self->output.frame+=MDLRS_ADSR_envelope(&self->envelope, oscillation->state, (float)oscillation->time/sample_rate)*oscillation->velocity*waveform(oscillation->phase);
	}
	oscillation=self->released; while (oscillation)
	{
		struct Oscillation *previous;
		chain:
		previous=oscillation->previous;
		switch (oscillation->state)
		{
			case ATTACK:
				if (oscillation->time!=(uint32_t)(sample_rate*self->envelope.attack.value)) break;
				oscillation->state=DECAY;
				oscillation->time=0;
			case DECAY:
				if (oscillation->time!=(uint32_t)(sample_rate*self->envelope.decay.value)) break;
			case SUSTAIN:
				oscillation->state=RELEASE;
				oscillation->time=0;
			case RELEASE:
				if (oscillation->time!=(uint32_t)(sample_rate*self->envelope.release.value)) break;
				oscillation->previous=self->unused;
				self->unused=oscillation;
				if (previous)
				{
					if (oscillation->next)
					{
						previous->next=oscillation->next;
						oscillation->next->previous=previous;
					}
					else
					{
						previous->next=NULL;
						self->released=previous;
					}
					oscillation=previous;
					goto chain;
				}
				else
				{
					if (oscillation->next) oscillation->next->previous=NULL;
					else self->released=NULL;
					self->output.frame*=self->volume.value;
					return;
				}
		}
		self->output.frame+=MDLRS_ADSR_envelope(&self->envelope, oscillation->state, (float)oscillation->time/sample_rate)*oscillation->velocity*waveform(oscillation->phase);
		oscillation=previous;
	}
	self->output.frame*=self->volume.value;
}
static float sine(float phase)
{
	return sinf(phase*2*(float)M_PI);
}
static float triangle(float phase)
{
	return 4*((phase>=.25f&&phase<.75f)*(.5f-phase)+(phase<.25f)*phase+(phase>=.75f)*(phase-1));
}
static float saw(float phase)
{
	return 1-2*phase;
}
static float square(float phase)
{
	return (phase<.5f)-(phase>=.5f);
}
void on_update(struct MDLRS_Oscillator *self)
{
	struct MDLRS_Note_event *event;
	struct Oscillation *oscillation;
	unsigned sample_rate=self->p.project->sample_rate;
	for (unsigned a=0; a!=self->input.event_count; a++)
	{
		event=self->input.events+a;
		switch (event->type)
		{
			case NOTE_START:
				if (self->unused)
				{
					oscillation=self->unused;
					self->unused=oscillation->previous;
				}
				else oscillation=safe_object(self->p.project, "Oscillation @ on_update()", sizeof(struct Oscillation));
				oscillation->state=ATTACK;
				oscillation->phase_speed=event->pitch/sample_rate;
				oscillation->velocity=event->velocity;
				oscillation->phase=event->phase;
				oscillation->time=0;
				MDLRS_Note_table_set(&self->pressed_table, event->scancode, oscillation);
				if (self->pressed)
				{
					self->pressed->next=oscillation;
					oscillation->previous=self->pressed;
				}
				else oscillation->previous=NULL;
				oscillation->next=NULL;
				self->pressed=oscillation;
				break;
			case NOTE_CHANGE:
				oscillation=MDLRS_Note_table_get(&self->pressed_table, event->scancode);
				oscillation->phase_speed=event->pitch/sample_rate;
				oscillation->velocity=event->velocity;
				break;
			case NOTE_STOP:
				oscillation=MDLRS_Note_table_get(&self->pressed_table, event->scancode);
				MDLRS_Note_table_unset(&self->pressed_table, event->scancode);
				if (oscillation->previous)
				{
					if (oscillation->next)
					{
						oscillation->previous->next=oscillation->next;
						oscillation->next->previous=oscillation->previous;
					}
					else
					{
						oscillation->previous->next=NULL;
						self->pressed=oscillation->previous;
					}
				}
				else if (oscillation->next) oscillation->next->previous=NULL;
				else self->pressed=NULL;
				if (self->released)
				{
					self->released->next=oscillation;
					oscillation->previous=self->released;
				}
				else oscillation->previous=NULL;
				oscillation->next=NULL;
				self->released=oscillation;
		}
	}
	switch (self->waveform.value)
	{
		case 0:
			generate(self, sine);
			break;
		case 1:
			generate(self, triangle);
			break;
		case 2:
			generate(self, saw);
			break;
		case 3:
			generate(self, square);
	}
	for (oscillation=self->pressed; oscillation; oscillation=oscillation->previous)
	{
		oscillation->time++;
		if ((oscillation->phase+=oscillation->phase_speed)>=1) oscillation->phase--;
	}
	for (oscillation=self->released; oscillation; oscillation=oscillation->previous)
	{
		oscillation->time++;
		if ((oscillation->phase+=oscillation->phase_speed)>=1) oscillation->phase--;
	}
}
void MDLRS_Oscillator_set_volume(struct MDLRS_Oscillator *self, float volume)
{
	self->volume.value=volume;
}
void MDLRS_Oscillator_set_waveform(struct MDLRS_Oscillator *self, unsigned waveform)
{
	self->waveform.value=waveform;
}
struct MDLRS_Note *MDLRS_Oscillator_get_input(struct MDLRS_Oscillator *self)
{
	return &self->input;
}
struct MDLRS_Real_controller *MDLRS_Oscillator_get_volume(struct MDLRS_Oscillator *self)
{
	return &self->volume;
}
struct MDLRS_Integer_controller *MDLRS_Oscillator_get_waveform(struct MDLRS_Oscillator *self)
{
	return &self->waveform;
}
struct MDLRS_ADSR *MDLRS_Oscillator_get_envelope(struct MDLRS_Oscillator *self)
{
	return &self->envelope;
}
struct MDLRS_Sound *MDLRS_Oscillator_get_output(struct MDLRS_Oscillator *self)
{
	return &self->output;
}
void MDLRS_Oscillator_deinit(struct MDLRS_Oscillator *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Note_deinit(&self->input);
	MDLRS_Port_deinit(&self->volume.p);
	MDLRS_Port_deinit(&self->waveform.p);
	MDLRS_ADSR_deinit(&self->envelope);
	MDLRS_Port_deinit(&self->output.p);
	MDLRS_Note_table_deinit(&self->pressed_table);
	struct Oscillation *previous;
	struct Oscillation *a=self->pressed; while (a)
	{
		previous=a->previous;
		safe_dispose(self->p.project, a);
		a=previous;
	}
	a=self->released; while (a)
	{
		previous=a->previous;
		safe_dispose(self->p.project, a);
		a=previous;
	}
	a=self->unused; while (a)
	{
		previous=a->previous;
		safe_dispose(self->p.project, a);
		a=previous;
	}

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Oscillator_remove(struct MDLRS_Oscillator *self)
{
	MDLRS_Oscillator_deinit(self);
	safe_dispose(self->p.project, self);
}