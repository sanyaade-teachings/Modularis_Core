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

#include <Modularis_Core/Modularis.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <system/safe memory.h>
#include <system/modules/Output.h>
#include <system/modules/Module.h>
#include <ports/Sound.h>
#include <Modularis_Core/system/Safe_block.h>

struct MDLRS_Modularis *MDLRS_Modularis_allocate()
{
	return malloc(sizeof(struct MDLRS_Modularis));
}
struct MDLRS_Modularis *MDLRS_Modularis_new(unsigned sample_rate, unsigned channels)
{
	struct MDLRS_Modularis *result=malloc(sizeof(struct MDLRS_Modularis));
	MDLRS_Modularis_init(result, sample_rate, channels);
	return result;
}
void MDLRS_Modularis_init(struct MDLRS_Modularis *self, unsigned sample_rate, unsigned channels)
{
	self->sample_rate=sample_rate;
	self->lazy_update=true;
	self->dispose_leaks=true;
	self->last_block=NULL;
	self->block_count=0;
	self->max_block_count=0;
	self->allocated=0;
	self->max_allocated=0;
	self->disconnected_modules=safe_array(self, "MDLRS_Module * @ MDLRS_Modularis_init()", sizeof(struct MDLRS_Module *), 8);
	self->disconnected_module_count=0;
	self->note_scancode=0;
	self->output=MDLRS_Output_new(self, channels);
}
void MDLRS_Modularis_update(struct MDLRS_Modularis *self)
{
	MDLRS_Module_update(&self->output->p);
	if (!self->lazy_update) for (unsigned a=0; a!=self->disconnected_module_count; a++) MDLRS_Module_update(self->disconnected_modules[a]);
	MDLRS_Module_get_ready(&self->output->p);
	if (!self->lazy_update) for (unsigned a=0; a!=self->disconnected_module_count; a++) MDLRS_Module_get_ready(self->disconnected_modules[a]);
}
MDLRS_Sound_value MDLRS_Modularis_get(struct MDLRS_Modularis *self, unsigned channel)
{
	return self->output->channels[channel].frame;
}
void MDLRS_Modularis_deinit(struct MDLRS_Modularis *self)
{
	safe_dispose(self, self->disconnected_modules);
	MDLRS_Output_remove(self->output);
	if (self->dispose_leaks)
	{
		struct MDLRS_Safe_block *a=self->last_block; while (a)
		{
			struct MDLRS_Safe_block *previous=a->previous;
			free(a);
			a=previous;
		}
	}
}
void MDLRS_Modularis_remove(struct MDLRS_Modularis *self)
{
	self->dispose_leaks=true;
	MDLRS_Modularis_deinit(self);
	free(self);
}
void MDLRS_Modularis_dispose(struct MDLRS_Modularis *self)
{
	if (!self->dispose_leaks)
	{
		struct MDLRS_Safe_block *a=self->last_block; while (a)
		{
			struct MDLRS_Safe_block *previous=a->previous;
			free(a);
			a=previous;
		}
	}
	free(self);
}