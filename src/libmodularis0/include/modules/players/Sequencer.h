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

#pragma once

#include <Modularis_Core/system/interfaces/modules/players/Sequencer.h>
#include <system/modules/Module.h>

#include <ports/controllers/Real_controller.h>
#include <ports/controllers/Integer_controller.h>
#include <ports/Note.h>
#include <stdint.h>
#include <stdbool.h>

struct MDLRS_Pattern;

struct Pattern_sequence_data
{
	unsigned count;
	unsigned pattern;
	float time;
};
struct Pattern_data
{
	bool pressed;
	unsigned scancode;
	unsigned note, velocity, phase;
	float note_time, velocity_time, phase_time;
};
struct MDLRS_Sequencer
{
	struct MDLRS_Module p;

	struct MDLRS_Real_controller BPM;
	struct MDLRS_Integer_controller LPB;
	struct MDLRS_Real_controller cursor_position;
	struct MDLRS_Integer_controller loop;
	struct MDLRS_Integer_controller play;
	struct MDLRS_Note output;
	struct MDLRS_Pattern ***tracks;
	struct Pattern_sequence_data *track_data;
	struct Pattern_data *pattern_data;
	unsigned track_count;
	unsigned sequence_count;
	uint32_t time;
	bool playing;
	bool position_changed;
};