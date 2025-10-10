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

#include <modules/players/Sequencer.h>

#include <system/safe memory.h>
#include <stddef.h>
#include <stdbool.h>
#include <Modularis_Core/user/modules/players/Sequencer/Pattern.h>
#include <Modularis_Core/Modularis.h>
#include <Modularis_Core/user/modules/players/Sequencer/Sequence.h>
#include <Modularis_Core/user/modules/players/Sequencer/Note_key.h>
#include <Modularis_Core/user/modules/players/Sequencer/Continuous_key.h>
#include <Modularis_Core/user/modules/players/Sequencer/Discrete_key.h>
#include <math.h>

static void on_update(struct MDLRS_Sequencer *self);
static const struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Sequencer *MDLRS_Sequencer_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Sequencer", sizeof(struct MDLRS_Sequencer));
}
struct MDLRS_Sequencer *MDLRS_Sequencer_new(struct MDLRS_Modularis *project)
{
	struct MDLRS_Sequencer *result=MDLRS_Sequencer_allocate(project);
	MDLRS_Sequencer_init(result, project);
	return result;
}
void MDLRS_Sequencer_init(struct MDLRS_Sequencer *self, struct MDLRS_Modularis *project)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Real_controller_init(&self->BPM, &self->p, 120);
	MDLRS_Integer_controller_init(&self->LPB, &self->p, 8);
	MDLRS_Real_controller_init(&self->cursor_position, &self->p, 0);
	MDLRS_Integer_controller_init(&self->loop, &self->p, 0);
	MDLRS_Integer_controller_init(&self->play, &self->p, 0);
	MDLRS_Note_init(&self->output, &self->p);
	MDLRS_Port_group_add(&self->p.inputs, &self->BPM.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->LPB.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->cursor_position.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->loop.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->play.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
	self->tracks=NULL;
	self->track_data=NULL;
	self->pattern_data=NULL;
	self->track_count=0;
	self->sequence_count=0;
	self->time=0;
	self->playing=false;
	self->position_changed=false;
}
static void change_notes(struct MDLRS_Note *port, struct MDLRS_Sequence *track, struct Pattern_data *track_data, float time)
{
	while (time>=track_data->note_time||time>=track_data->velocity_time||time>=track_data->phase_time)
	{
		if (track_data->note_time<track_data->phase_time)
		{
			if (track_data->note_time<track_data->velocity_time)
			{
				track_data->note++;
				if (track_data->pressed)
				{
					if (track->notes[track_data->note].pressed) MDLRS_Note_add_change(port, track_data->scancode, 440*powf(2, track->notes[track_data->note].p.value/12), MDLRS_Continuous_key_get_value(track->velocities+track_data->velocity, track_data->note_time+track->velocities[track_data->velocity].duration-track_data->velocity_time));
					else
					{
						MDLRS_Note_add_stop(port, track_data->scancode);
						track_data->pressed=false;
					}
				}
				else if (track->notes[track_data->note].pressed)
				{
					track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, track->notes[track_data->note].p.value/12), MDLRS_Continuous_key_get_value(track->velocities+track_data->velocity, track_data->note_time+track->velocities[track_data->velocity].duration-track_data->velocity_time), track->phases[track_data->phase].value);
					track_data->pressed=true;
				}
				track_data->note_time+=track->notes[track_data->note].p.duration;
			}
			else if (track_data->velocity_time<track_data->note_time)
			{
				track_data->velocity++;
				if (track_data->pressed) MDLRS_Note_add_change(port, track_data->scancode, 440*powf(2, MDLRS_Note_key_get_value(track->notes+track_data->note, track_data->velocity_time+track->notes[track_data->note].p.duration-track_data->note_time)/12), track->velocities[track_data->velocity].value);
				track_data->velocity_time+=track->velocities[track_data->velocity].duration;
			}
			else
			{
				track_data->note++;
				track_data->velocity++;
				if (track_data->pressed)
				{
					if (track->notes[track_data->note].pressed) MDLRS_Note_add_change(port, track_data->scancode, 440*powf(2, track->notes[track_data->note].p.value/12), track->velocities[track_data->velocity].value);
					else
					{
						MDLRS_Note_add_stop(port, track_data->scancode);
						track_data->pressed=false;
					}
				}
				else if (track->notes[track_data->note].pressed)
				{
					track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, track->notes[track_data->note].p.value/12), track->velocities[track_data->velocity].value, track->phases[track_data->phase].value);
					track_data->pressed=true;
				}
				track_data->note_time+=track->notes[track_data->note].p.duration;
				track_data->velocity_time+=track->velocities[track_data->velocity].duration;
			}
		}
		else if (track_data->phase_time<track_data->note_time)
		{
			if (track_data->phase_time<track_data->velocity_time)
			{
				track_data->phase++;
				if (track_data->pressed)
				{
					MDLRS_Note_add_stop(port, track_data->scancode);
					track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, MDLRS_Note_key_get_value(track->notes+track_data->note, track_data->phase_time+track->notes[track_data->note].p.duration-track_data->note_time)/12), MDLRS_Continuous_key_get_value(track->velocities+track_data->velocity, track_data->phase_time+track->velocities[track_data->velocity].duration-track_data->velocity_time), track->phases[track_data->phase].value);
				}
				track_data->phase_time+=track->phases[track_data->phase].duration;
			}
			else if (track_data->velocity_time<track_data->phase_time)
			{
				track_data->velocity++;
				if (track_data->pressed) MDLRS_Note_add_change(port, track_data->scancode, 440*powf(2, MDLRS_Note_key_get_value(track->notes+track_data->note, track_data->velocity_time+track->notes[track_data->note].p.duration-track_data->note_time)/12), track->velocities[track_data->velocity].value);
				track_data->velocity_time+=track->velocities[track_data->velocity].duration;
			}
			else
			{
				track_data->velocity++;
				track_data->phase++;
				if (track_data->pressed)
				{
					MDLRS_Note_add_stop(port, track_data->scancode);
					track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, MDLRS_Note_key_get_value(track->notes+track_data->note, track_data->velocity_time+track->notes[track_data->note].p.duration-track_data->note_time)/12), track->velocities[track_data->velocity].value, track->phases[track_data->phase].value);
				}
				track_data->velocity_time+=track->velocities[track_data->velocity].duration;
				track_data->phase_time+=track->phases[track_data->phase].duration;
			}
		}
		else if (track_data->note_time<track_data->velocity_time)
		{
			track_data->note++;
			track_data->phase++;
			if (track_data->pressed)
			{
				if (track->notes[track_data->note].pressed)
				{
					MDLRS_Note_add_stop(port, track_data->scancode);
					track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, track->notes[track_data->note].p.value/12), MDLRS_Continuous_key_get_value(track->velocities+track_data->velocity, track_data->note_time+track->velocities[track_data->velocity].duration-track_data->velocity_time), track->phases[track_data->phase].value);
				}
				else
				{
					MDLRS_Note_add_stop(port, track_data->scancode);
					track_data->pressed=false;
				}
			}
			else if (track->notes[track_data->note].pressed)
			{
				track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, track->notes[track_data->note].p.value/12), MDLRS_Continuous_key_get_value(track->velocities+track_data->velocity, track_data->note_time+track->velocities[track_data->velocity].duration-track_data->velocity_time), track->phases[track_data->phase].value);
				track_data->pressed=true;
			}
			track_data->note_time+=track->notes[track_data->note].p.duration;
			track_data->phase_time+=track->phases[track_data->phase].duration;
		}
		else if (track_data->velocity_time<track_data->note_time)
		{
			track_data->velocity++;
			if (track_data->pressed) MDLRS_Note_add_change(port, track_data->scancode, 440*powf(2, MDLRS_Note_key_get_value(track->notes+track_data->note, track_data->velocity_time+track->notes[track_data->note].p.duration-track_data->note_time)/12), track->velocities[track_data->velocity].value);
			track_data->velocity_time+=track->velocities[track_data->velocity].duration;
		}
		else
		{
			track_data->note++;
			track_data->velocity++;
			track_data->phase++;
			if (track_data->pressed)
			{
				if (track->notes[track_data->note].pressed)
				{
					MDLRS_Note_add_stop(port, track_data->scancode);
					track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, track->notes[track_data->note].p.value/12), track->velocities[track_data->velocity].value, track->phases[track_data->phase].value);
				}
				else
				{
					MDLRS_Note_add_stop(port, track_data->scancode);
					track_data->pressed=false;
				}
			}
			else if (track->notes[track_data->note].pressed)
			{
				track_data->scancode=MDLRS_Note_add_start(port, 440*powf(2, track->notes[track_data->note].p.value/12), track->velocities[track_data->velocity].value, track->phases[track_data->phase].value);
				track_data->pressed=true;
			}
			track_data->note_time+=track->notes[track_data->note].p.duration;
			track_data->velocity_time+=track->velocities[track_data->velocity].duration;
			track_data->phase_time+=track->phases[track_data->phase].duration;
		}
	}
}
void on_update(struct MDLRS_Sequencer *self)
{
	self->output.event_count=0;
	if (self->cursor_position.p.connection_count) self->position_changed=true;
	else
	{
		struct Pattern_data *sequence;
		if (self->play.value)
		{
			if (self->position_changed)
			{
				sequence=self->pattern_data;
				if (self->playing)
				{
					for (unsigned a=0; a!=self->sequence_count; a++) if (sequence[a].pressed)
					{
						MDLRS_Note_add_stop(&self->output, sequence[a].scancode);
						sequence[a].pressed=false;
					}
				}
				self->time=60*self->p.project->sample_rate*self->cursor_position.value/(self->LPB.value*self->BPM.value);
				for (unsigned a=0; a!=self->track_count; a++)
				{
					struct MDLRS_Pattern **track=self->tracks[a];
					struct Pattern_sequence_data *track_data=self->track_data+a;
					track_data->pattern=0;
					track_data->time=0;
					float time;
					while (track[track_data->pattern])
					{
						time=track_data->time+track[track_data->pattern]->length;
						if (self->cursor_position.value<time) break;
						track_data->time=time;
						track_data->pattern++;
					}
					struct MDLRS_Pattern *pattern=track[track_data->pattern];
					if (!pattern)
					{
						self->cursor_position.value=0;
						self->time=0;
						if (track_data->pattern&&self->loop.value)
						{
							loop:
							for (sequence=self->pattern_data, a=0; a!=self->track_count; sequence+=self->track_data[a].count, a++)
							{
								pattern=*self->tracks[a];
								self->track_data[a].pattern=0;
								self->track_data[a].time=pattern->length;
								for (unsigned a=0; a!=pattern->track_count; a++)
								{
									struct Pattern_data *track_data=sequence+a;
									track_data->note=-1;
									track_data->velocity=-1;
									track_data->phase=-1;
									track_data->note_time=0;
									track_data->velocity_time=0;
									track_data->phase_time=0;
								}
							}
							break;
						}
						self->play.value=0;
						self->playing=false;
						return;
					}
					time=self->cursor_position.value-track_data->time;
					for (unsigned a=0; a!=pattern->track_count; a++)
					{
						struct MDLRS_Sequence *track=pattern->tracks+a;
						struct Pattern_data *track_data=sequence+a;
						track_data->note=0;
						track_data->velocity=0;
						track_data->phase=0;
						track_data->note_time=0;
						track_data->velocity_time=0;
						track_data->phase_time=0;
						while (time>track_data->note_time)
						{
							track_data->note_time+=track->notes[track_data->note].p.duration;
							track_data->note++;
						}
						while (time>track_data->velocity_time)
						{
							track_data->velocity_time+=track->velocities[track_data->velocity].duration;
							track_data->velocity++;
						}
						while (time>track_data->phase_time)
						{
							track_data->phase_time+=track->phases[track_data->phase].duration;
							track_data->phase++;
						}
						track_data->note--;
						track_data->velocity--;
						track_data->phase--;
					}
					track_data->time+=pattern->length;
					sequence+=track_data->count;
				}
				self->position_changed=false;
			}
			self->playing=true;
			sequence=self->pattern_data;
			for (unsigned a=0; a!=self->track_count; a++)
			{
				struct MDLRS_Pattern **track=self->tracks[a];
				struct Pattern_sequence_data *track_data=self->track_data+a;
				struct MDLRS_Pattern *pattern=track[track_data->pattern];
				float time=self->cursor_position.value+pattern->length-track_data->time;
				while (self->cursor_position.value>=track_data->time)
				{
					if (!track[track_data->pattern+1])
					{
						self->output.event_count=0;
						sequence=self->pattern_data;
						for (a=0; a!=self->sequence_count; a++) if (sequence[a].pressed)
						{
							MDLRS_Note_add_stop(&self->output, sequence[a].scancode);
							sequence[a].pressed=false;
						}
						self->cursor_position.value=0;
						self->time=0;
						if (self->loop.value) goto loop;
						self->play.value=0;
						self->playing=false;
						self->position_changed=true;
						return;
					}
					for (unsigned a=0; a!=pattern->track_count; a++) if (sequence[a].pressed)
					{
						MDLRS_Note_add_stop(&self->output, sequence[a].scancode);
						sequence[a].pressed=false;
					}
					time=self->cursor_position.value-track_data->time;
					pattern=track[++track_data->pattern];
					track_data->time+=pattern->length;
					for (unsigned a=0; a!=pattern->track_count; a++)
					{
						struct MDLRS_Sequence *track=pattern->tracks+a;
						struct Pattern_data *track_data=sequence+a;
						track_data->note=-1;
						track_data->velocity=-1;
						track_data->phase=-1;
						track_data->note_time=0;
						track_data->velocity_time=0;
						track_data->phase_time=0;
						change_notes(&self->output, track, track_data, time);
					}
				}
				for (unsigned a=0; a!=pattern->track_count; a++)
				{
					struct MDLRS_Sequence *track=pattern->tracks+a;
					struct Pattern_data *track_data=sequence+a;
					change_notes(&self->output, track, track_data, time);
					if (track_data->pressed) if (track->notes[track_data->note].p.curve!=INTERPOLATION_NONE||track->velocities[track_data->velocity].curve!=INTERPOLATION_NONE)
					{
						MDLRS_Note_add_change(&self->output, track_data->scancode, 440*powf(2, MDLRS_Note_key_get_value(track->notes+track_data->note, time+track->notes[track_data->note].p.duration-track_data->note_time)/12), MDLRS_Continuous_key_get_value(track->velocities+track_data->velocity, time+track->velocities[track_data->velocity].duration-track_data->velocity_time));
					}
				}
				sequence+=track_data->count;
			}
			self->time++;
			self->cursor_position.value=self->LPB.value*self->BPM.value*(float)self->time/(60*self->p.project->sample_rate);
		}
		else if (self->playing)
		{
			sequence=self->pattern_data;
			for (unsigned a=0; a!=self->sequence_count; a++) if (sequence[a].pressed)
			{
				MDLRS_Note_add_stop(&self->output, sequence[a].scancode);
				sequence[a].pressed=false;
			}
			self->playing=false;
		}
	}
}
void MDLRS_Sequencer_set_BPM(struct MDLRS_Sequencer *self, float BPM)
{
	self->BPM.value=BPM;
}
void MDLRS_Sequencer_set_LPB(struct MDLRS_Sequencer *self, float LPB)
{
	self->LPB.value=LPB;
}
void MDLRS_Sequencer_set_position(struct MDLRS_Sequencer *self, float cursor_position)
{
	self->cursor_position.value=cursor_position;
	self->position_changed=true;
}
void MDLRS_Sequencer_set_loop(struct MDLRS_Sequencer *self, bool loop)
{
	self->loop.value=loop;
}
void MDLRS_Sequencer_set_play(struct MDLRS_Sequencer *self, bool play)
{
	self->play.value=play;
}
struct MDLRS_Real_controller *MDLRS_Sequencer_get_BPM(struct MDLRS_Sequencer *self)
{
	return &self->BPM;
}
struct MDLRS_Integer_controller *MDLRS_Sequencer_get_LPB(struct MDLRS_Sequencer *self)
{
	return &self->LPB;
}
struct MDLRS_Real_controller *MDLRS_Sequencer_get_position(struct MDLRS_Sequencer *self)
{
	return &self->cursor_position;
}
struct MDLRS_Integer_controller *MDLRS_Sequencer_get_loop(struct MDLRS_Sequencer *self)
{
	return &self->loop;
}
struct MDLRS_Integer_controller *MDLRS_Sequencer_get_play(struct MDLRS_Sequencer *self)
{
	return &self->play;
}
struct MDLRS_Note *MDLRS_Sequencer_get_output(struct MDLRS_Sequencer *self)
{
	return &self->output;
}
void MDLRS_Sequencer_add(struct MDLRS_Sequencer *self, struct MDLRS_Pattern ***tracks, unsigned track_count)
{
	struct MDLRS_Modularis *project=self->p.project;
	struct Pattern_sequence_data *track_data;
	if (self->track_data) safe_dispose(project, self->track_data);
	track_data=safe_array(project, "Pattern_sequence_data @ MDLRS_Sequencer_add()", sizeof(struct Pattern_sequence_data), track_count);
	unsigned sequence_count=0;
	for (unsigned a=0; a!=track_count; a++)
	{
		unsigned max=0;
		for (unsigned b=0; tracks[a][b]; b++)
		{
			unsigned value=tracks[a][b]->track_count;
			if (value>max) max=value;
		}
		track_data[a].count=max;
		sequence_count+=max;
	}
	struct Pattern_data *pattern_data;
	if (self->pattern_data) safe_dispose(project, self->pattern_data);
	pattern_data=safe_array(project, "Pattern_data @ MDLRS_Sequencer_add()", sizeof(struct Pattern_data), sequence_count);
	safe_clean(pattern_data);
	self->tracks=tracks;
	self->track_data=track_data;
	self->pattern_data=pattern_data;
	self->track_count=track_count;
	self->sequence_count=sequence_count;
	self->position_changed=true;
}
void MDLRS_Sequencer_deinit(struct MDLRS_Sequencer *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->BPM.p);
	MDLRS_Port_deinit(&self->LPB.p);
	MDLRS_Port_deinit(&self->cursor_position.p);
	MDLRS_Port_deinit(&self->loop.p);
	MDLRS_Port_deinit(&self->play.p);
	MDLRS_Note_deinit(&self->output);
	if (self->track_data) safe_dispose(self->p.project, self->track_data);
	if (self->pattern_data) safe_dispose(self->p.project, self->pattern_data);

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Sequencer_remove(struct MDLRS_Sequencer *self)
{
	MDLRS_Sequencer_deinit(self);
	safe_dispose(self->p.project, self);
}