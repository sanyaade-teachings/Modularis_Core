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

#include <stdbool.h>

struct MDLRS_Sequencer;

struct MDLRS_Modularis;
struct MDLRS_Real_controller;
struct MDLRS_Integer_controller;
struct MDLRS_Note;
struct MDLRS_Pattern;

struct MDLRS_Sequencer *MDLRS_Sequencer_new(struct MDLRS_Modularis *project);
void MDLRS_Sequencer_init(struct MDLRS_Sequencer *self, struct MDLRS_Modularis *project);
void
	MDLRS_Sequencer_set_BPM(struct MDLRS_Sequencer *self, float BPM),
	MDLRS_Sequencer_set_LPB(struct MDLRS_Sequencer *self, float LPB),
	MDLRS_Sequencer_set_position(struct MDLRS_Sequencer *self, float cursor_position),
	MDLRS_Sequencer_set_loop(struct MDLRS_Sequencer *self, bool loop),
	MDLRS_Sequencer_set_play(struct MDLRS_Sequencer *self, bool play);
struct MDLRS_Real_controller *MDLRS_Sequencer_get_BPM(struct MDLRS_Sequencer *self);
struct MDLRS_Integer_controller *MDLRS_Sequencer_get_LPB(struct MDLRS_Sequencer *self);
struct MDLRS_Real_controller *MDLRS_Sequencer_get_position(struct MDLRS_Sequencer *self);
struct MDLRS_Integer_controller *MDLRS_Sequencer_get_loop(struct MDLRS_Sequencer *self);
struct MDLRS_Integer_controller *MDLRS_Sequencer_get_play(struct MDLRS_Sequencer *self);
struct MDLRS_Note *MDLRS_Sequencer_get_output(struct MDLRS_Sequencer *self);
void MDLRS_Sequencer_add(struct MDLRS_Sequencer *self, struct MDLRS_Pattern ***tracks, unsigned track_count);
void MDLRS_Sequencer_deinit(struct MDLRS_Sequencer *self);
void MDLRS_Sequencer_remove(struct MDLRS_Sequencer *self);