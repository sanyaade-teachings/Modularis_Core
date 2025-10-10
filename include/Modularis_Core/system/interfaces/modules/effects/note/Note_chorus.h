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

#include <stdbool.h>

struct MDLRS_Note_chorus;

struct MDLRS_Modularis;
struct MDLRS_Note;
struct MDLRS_Real_controller;
struct MDLRS_Integer_controller;

struct MDLRS_Note_chorus *MDLRS_Note_chorus_new(struct MDLRS_Modularis *project, float spread, unsigned voices);
void MDLRS_Note_chorus_init(struct MDLRS_Note_chorus *self, struct MDLRS_Modularis *project, float spread, unsigned voices);
void
	MDLRS_Note_chorus_set_spread(struct MDLRS_Note_chorus *self, float spread),
	MDLRS_Note_chorus_set_voices(struct MDLRS_Note_chorus *self, unsigned voices),
	MDLRS_Note_chorus_set_random_phases(struct MDLRS_Note_chorus *self, bool random_phases);
struct MDLRS_Note *MDLRS_Note_chorus_get_input(struct MDLRS_Note_chorus *self);
struct MDLRS_Real_controller *MDLRS_Note_chorus_get_spread(struct MDLRS_Note_chorus *self);
struct MDLRS_Integer_controller *MDLRS_Note_chorus_get_voices(struct MDLRS_Note_chorus *self);
struct MDLRS_Integer_controller *MDLRS_Note_chorus_get_random_phases(struct MDLRS_Note_chorus *self);
struct MDLRS_Note *MDLRS_Note_chorus_get_output(struct MDLRS_Note_chorus *self);
void MDLRS_Note_chorus_deinit(struct MDLRS_Note_chorus *self);
void MDLRS_Note_chorus_remove(struct MDLRS_Note_chorus *self);