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

#include <Sampler.h>

#include <stdlib.h>
#include <Modularis_Core/ports/Note.h>
#include <Modularis_Core/ports/controllers/Real_controller.h>
#include <Modularis_Core/ports/controllers/ADSR.h>
#include <Modularis_Core/ports/controllers/Integer_controller.h>
#include <Modularis_Core/system/modules/Module.h>
#include <Modularis_Core/ports/Port_group.h>
#include <stdio.h>
#include <stdint.h>
#include <Modularis_Core/system/types/Sound_value.h>
#include <Modularis_Core/ports/Sound.h>
#include <Modularis_Core/system/ports/Note/Note_table.h>
#include <stddef.h>
#include <Modularis_Core/system/ports/Note/Note_event.h>
#include <Sample.h>
#include <Modularis_Core/Modularis.h>
#include <string.h>
#include <stdbool.h>

static struct MDLRS_IModule_f f=
{
	(void (*)(void *))MDLRS_Sampler_on_update
};

struct MDLRS_Sampler *MDLRS_Sampler_new(struct MDLRS_Modularis *project, const char *file)
{
	struct MDLRS_Sampler *result=malloc(sizeof(struct MDLRS_Sampler));
	MDLRS_Sampler_init(result, project, file);
	return result;
}
void MDLRS_Sampler_init(struct MDLRS_Sampler *self, struct MDLRS_Modularis *project, const char *file)
{
	MDLRS_IModule_init(&self->p, project);
	self->p.f=&f;

	self->input=MDLRS_Note_new(self->p.p);
	self->volume=MDLRS_Real_controller_new(self->p.p, 1);
	self->envelope=MDLRS_ADSR_new(self->p.p, 0, 0, 1, 0);
	self->loop=MDLRS_Integer_controller_new(self->p.p, 0);
	self->loop_start=MDLRS_Integer_controller_new(self->p.p, 0);
	self->loop_length=MDLRS_Integer_controller_new(self->p.p, 0);
	struct MDLRS_Port_group *group=MDLRS_Module_get_inputs(self->p.p);
	MDLRS_Port_group_add(group, &self->input->p.p);
	MDLRS_Port_group_add(group, &self->volume->p.p);
	MDLRS_Port_group_add(group, &self->envelope->p.p);
	MDLRS_Port_group_add(group, &self->loop->p.p);
	MDLRS_Port_group_add(group, &self->loop_start->p.p);
	MDLRS_Port_group_add(group, &self->loop_length->p.p);
	FILE *sample_file=fopen(file, "rb");
	uint16_t format;
	uint16_t channels;
	uint32_t sample_rate;
	uint16_t bytes;
	uint16_t bits;
	uint32_t length;
	fseek(sample_file, 20, SEEK_CUR);
	fread(&format, 2, 1, sample_file);
	fread(&channels, 2, 1, sample_file);
	fread(&sample_rate, 4, 1, sample_file);
	fseek(sample_file, 4, SEEK_CUR);
	fread(&bytes, 2, 1, sample_file);
	fread(&bits, 2, 1, sample_file);
	fseek(sample_file, 4, SEEK_CUR);
	fread(&length, 4, 1, sample_file);
	length=channels*length/bytes;
	MDLRS_Sound_value *sample=malloc(sizeof(MDLRS_Sound_value)*(length+channels));
	switch (format)
	{
		case 1: //PCM
			switch (bits)
			{
				case 8:
					for (uint32_t a=0; a!=length; a++)
					{
						uint8_t frame;
						fread(&frame, 1, 1, sample_file);
						sample[a]=(MDLRS_Sound_value)frame/(uint8_t)(1<<7)-1;
					}
					break;
				case 16:
					for (uint32_t a=0; a!=length; a++)
					{
						int16_t frame;
						fread(&frame, 2, 1, sample_file);
						sample[a]=(MDLRS_Sound_value)frame/(uint16_t)(1<<15);
					}
					break;
				case 24:
					for (uint32_t a=0; a!=length; a++)
					{
						int32_t frame=0;
						fread(&frame, 3, 1, sample_file);
						sample[a]=(MDLRS_Sound_value)frame/(uint32_t)(1<<23);
					}
					break;
				case 32:
					for (uint32_t a=0; a!=length; a++)
					{
						int32_t frame;
						fread(&frame, 4, 1, sample_file);
						sample[a]=(MDLRS_Sound_value)frame/(uint32_t)(1<<31);
					}
			}
			break;
		case 3: //IEEE_FLOAT
			fread(sample, 4, length, sample_file);
	}
	fclose(sample_file);
	memset(sample+length, 0, sizeof(MDLRS_Sound_value)*channels);
	group=MDLRS_Module_get_outputs(self->p.p);
	struct MDLRS_Sound **outputs=malloc(sizeof(struct MDLRS_Sound *)*channels);
	for (uint16_t a=0; a!=channels; a++)
	{
		outputs[a]=MDLRS_Sound_new(self->p.p);
		MDLRS_Port_group_add(group, &outputs[a]->p.p);
	}
	self->outputs_array=outputs;
	self->pressed_table=MDLRS_Note_table_new(project);
	self->pressed=NULL;
	self->released=NULL;
	self->unused=NULL;
	self->sample=sample;
	self->frame=malloc(sizeof(MDLRS_Sound_value)*channels);
	self->length=length/channels;
	self->sample_rate=sample_rate;
	self->channels=channels;
}
void MDLRS_Sampler_on_update(struct MDLRS_Sampler *self)
{
	struct MDLRS_Note_event *event;
	struct Sample *sample;
	struct MDLRS_Note_event *events=MDLRS_Note_get_events(self->input);
	unsigned sample_rate=MDLRS_Module_get_project(self->p.p)->sample_rate;
	unsigned event_count=MDLRS_Note_get_event_count(self->input);
	for (unsigned a=0; a!=event_count; a++)
	{
		event=events+a;
		switch (event->type)
		{
			case NOTE_START:
				if (self->unused)
				{
					sample=self->unused;
					self->unused=sample->previous;
				}
				else sample=malloc(sizeof(struct Sample));
				sample->state=ATTACK;
				sample->speed=self->sample_rate*event->pitch/(440*sample_rate);
				sample->velocity=event->velocity;
				sample->frame=event->phase*self->length;
				sample->time=0;
				MDLRS_Note_table_set(self->pressed_table, event->scancode, sample);
				if (self->pressed)
				{
					self->pressed->next=sample;
					sample->previous=self->pressed;
				}
				else sample->previous=NULL;
				sample->next=NULL;
				self->pressed=sample;
				break;
			case NOTE_CHANGE:
				sample=MDLRS_Note_table_get(self->pressed_table, event->scancode);
				sample->speed=self->sample_rate*event->pitch/(440*sample_rate);
				sample->velocity=event->velocity;
				break;
			case NOTE_STOP:
				sample=MDLRS_Note_table_get(self->pressed_table, event->scancode);
				MDLRS_Note_table_unset(self->pressed_table, event->scancode);
				if (sample->previous)
				{
					if (sample->next)
					{
						sample->previous->next=sample->next;
						sample->next->previous=sample->previous;
					}
					else
					{
						sample->previous->next=NULL;
						self->pressed=sample->previous;
					}
				}
				else if (sample->next) sample->next->previous=NULL;
				else self->pressed=NULL;
				if (self->released)
				{
					self->released->next=sample;
					sample->previous=self->released;
				}
				else sample->previous=NULL;
				sample->next=NULL;
				self->released=sample;
		}
	}
	float attack=MDLRS_Real_controller_get(MDLRS_ADSR_get_attack(self->envelope));
	float decay=MDLRS_Real_controller_get(MDLRS_ADSR_get_decay(self->envelope));
	float sustain=MDLRS_Real_controller_get(MDLRS_ADSR_get_sustain(self->envelope));
	float release=MDLRS_Real_controller_get(MDLRS_ADSR_get_release(self->envelope));
	MDLRS_Sound_value *frame=self->frame;
	memset(frame, 0, sizeof(MDLRS_Sound_value)*self->channels);
	for (sample=self->pressed; sample; sample=sample->previous)
	{
		switch (sample->state)
		{
			case ATTACK:
				if (sample->time!=(uint32_t)(sample_rate*attack)) break;
				sample->state=DECAY;
				sample->time=0;
			case DECAY:
				if (sample->time!=(uint32_t)(sample_rate*decay)) break;
				sample->state=SUSTAIN;
		}
		for (unsigned a=0; a!=self->channels; a++) frame[a]+=MDLRS_ADSR_envelope(self->envelope, sample->state, (float)sample->time/sample_rate)*sample->velocity*self->sample[self->channels*(uint32_t)sample->frame+a];
	}
	sample=self->released; while (sample)
	{
		struct Sample *previous;
		chain:
		previous=sample->previous;
		switch (sample->state)
		{
			case ATTACK:
				if (sample->time!=(uint32_t)(sample_rate*attack)) break;
				sample->state=DECAY;
				sample->time=0;
			case DECAY:
				if (sample->time!=(uint32_t)(sample_rate*decay)) break;
			case SUSTAIN:
				sample->state=RELEASE;
				sample->time=0;
			case RELEASE:
				if (sample->time!=(uint32_t)(sample_rate*release)) break;
				sample->previous=self->unused;
				self->unused=sample;
				if (previous)
				{
					if (sample->next)
					{
						previous->next=sample->next;
						sample->next->previous=previous;
					}
					else
					{
						previous->next=NULL;
						self->released=previous;
					}
					sample=previous;
					goto chain;
				}
				else
				{
					if (sample->next) sample->next->previous=NULL;
					else self->released=NULL;
					goto end;
				}
		}
		for (unsigned a=0; a!=self->channels; a++) frame[a]+=MDLRS_ADSR_envelope(self->envelope, sample->state, (float)sample->time/sample_rate)*sample->velocity*self->sample[self->channels*(uint32_t)sample->frame+a];
		sample=previous;
	}
	float volume;
	end:
	volume=MDLRS_Real_controller_get(self->volume);
	for (unsigned a=0; a!=self->channels; a++) MDLRS_Sound_set(self->outputs_array[a], frame[a]*volume);
	if (MDLRS_Integer_controller_get(self->loop))
	{
		uint32_t loop_start=MDLRS_Integer_controller_get(self->loop_start);
		uint32_t loop_length=MDLRS_Integer_controller_get(self->loop_length);
		for (sample=self->pressed; sample; sample=sample->previous)
		{
			sample->time++;
			if ((uint32_t)(sample->frame+=sample->speed)>=loop_start+loop_length) sample->frame-=loop_length;
		}
		for (sample=self->released; sample; sample=sample->previous)
		{
			sample->time++;
			if ((uint32_t)(sample->frame+=sample->speed)>=loop_start+loop_length) sample->frame-=loop_length;
		}
	}
	else
	{
		for (sample=self->pressed; sample; sample=sample->previous)
		{
			sample->time++;
			if ((uint32_t)(sample->frame+=sample->speed)>self->length) sample->frame=self->length;
		}
		for (sample=self->released; sample; sample=sample->previous)
		{
			sample->time++;
			if ((uint32_t)(sample->frame+=sample->speed)>self->length) sample->frame=self->length;
		}
	}
}
void MDLRS_Sampler_set_volume(struct MDLRS_Sampler *self, float volume)
{
	MDLRS_Real_controller_set(self->volume, volume);
}
void MDLRS_Sampler_set_loop(struct MDLRS_Sampler *self, uint32_t start, uint32_t length)
{
	if (length)
	{
		MDLRS_Integer_controller_set(self->loop, 1);
		MDLRS_Integer_controller_set(self->loop_start, start);
		MDLRS_Integer_controller_set(self->loop_length, length);
	}
	else MDLRS_Integer_controller_set(self->loop, 0);
}
struct MDLRS_ADSR *MDLRS_Sampler_get_envelope(struct MDLRS_Sampler *self)
{
	return self->envelope;
}
void MDLRS_Sampler_deinit(struct MDLRS_Sampler *self)
{
	MDLRS_Module_disconnect(self->p.p);
	MDLRS_Note_remove(self->input);
	MDLRS_Real_controller_remove(self->volume);
	MDLRS_ADSR_remove(self->envelope);
	MDLRS_Integer_controller_remove(self->loop);
	MDLRS_Integer_controller_remove(self->loop_start);
	MDLRS_Integer_controller_remove(self->loop_length);
	for (unsigned a=0; a!=self->channels; a++) MDLRS_Sound_remove(self->outputs_array[a]);
	free(self->outputs_array);
	MDLRS_Note_table_remove(self->pressed_table);
	struct Sample *previous;
	struct Sample *a=self->pressed; while (a)
	{
		previous=a->previous;
		free(a);
		a=previous;
	}
	a=self->released; while (a)
	{
		previous=a->previous;
		free(a);
		a=previous;
	}
	a=self->unused; while (a)
	{
		previous=a->previous;
		free(a);
		a=previous;
	}
	free(self->sample);
	free(self->frame);

	MDLRS_IModule_deinit(&self->p);
}
void MDLRS_Sampler_remove(struct MDLRS_Sampler *self)
{
	MDLRS_Sampler_deinit(self);
	free(self);
}