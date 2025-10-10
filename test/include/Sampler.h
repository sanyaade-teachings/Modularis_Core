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

#pragma once

#include <Modularis_Core/extensions/modules/IModule.h>

#include <Modularis_Core/system/types/Sound_value.h>
#include <stdint.h>

struct MDLRS_Note;
struct MDLRS_Real_controller;
struct MDLRS_ADSR;
struct MDLRS_Integer_controller;
struct MDLRS_Sound;
struct MDLRS_Note_table;
struct Sample;

struct MDLRS_Sampler
{
	struct MDLRS_IModule p;

	struct MDLRS_Note *input;
	struct MDLRS_Real_controller *volume;
	struct MDLRS_ADSR *envelope;
	struct MDLRS_Integer_controller *loop;
	struct MDLRS_Integer_controller *loop_start;
	struct MDLRS_Integer_controller *loop_length;
	struct MDLRS_Sound **outputs_array;
	struct MDLRS_Note_table *pressed_table;
	struct Sample *pressed, *released, *unused;
	MDLRS_Sound_value *sample;
	MDLRS_Sound_value *frame;
	uint32_t length;
	unsigned sample_rate;
	unsigned channels;
};
struct MDLRS_Sampler *MDLRS_Sampler_new(struct MDLRS_Modularis *project, const char *file);
void MDLRS_Sampler_init(struct MDLRS_Sampler *self, struct MDLRS_Modularis *project, const char *file);
void MDLRS_Sampler_on_update(struct MDLRS_Sampler *self);
void MDLRS_Sampler_set_volume(struct MDLRS_Sampler *self, float volume);
void MDLRS_Sampler_set_loop(struct MDLRS_Sampler *self, uint32_t start, uint32_t length);
struct MDLRS_ADSR *MDLRS_Sampler_get_envelope(struct MDLRS_Sampler *self);
void MDLRS_Sampler_deinit(struct MDLRS_Sampler *self);
void MDLRS_Sampler_remove(struct MDLRS_Sampler *self);