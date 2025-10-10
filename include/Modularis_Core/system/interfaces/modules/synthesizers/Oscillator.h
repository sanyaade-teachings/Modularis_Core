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

struct MDLRS_Oscillator;

struct MDLRS_Modularis;
struct MDLRS_Note;
struct MDLRS_Real_controller;
struct MDLRS_Integer_controller;
struct MDLRS_ADSR;
struct MDLRS_Sound;

struct MDLRS_Oscillator *MDLRS_Oscillator_new(struct MDLRS_Modularis *project);
void MDLRS_Oscillator_init(struct MDLRS_Oscillator *self, struct MDLRS_Modularis *project);
void MDLRS_Oscillator_set_volume(struct MDLRS_Oscillator *self, float volume);
void MDLRS_Oscillator_set_waveform(struct MDLRS_Oscillator *self, unsigned waveform);
struct MDLRS_Note *MDLRS_Oscillator_get_input(struct MDLRS_Oscillator *self);
struct MDLRS_Real_controller *MDLRS_Oscillator_get_volume(struct MDLRS_Oscillator *self);
struct MDLRS_Integer_controller *MDLRS_Oscillator_get_waveform(struct MDLRS_Oscillator *self);
struct MDLRS_ADSR *MDLRS_Oscillator_get_envelope(struct MDLRS_Oscillator *self);
struct MDLRS_Sound *MDLRS_Oscillator_get_output(struct MDLRS_Oscillator *self);
void MDLRS_Oscillator_deinit(struct MDLRS_Oscillator *self);
void MDLRS_Oscillator_remove(struct MDLRS_Oscillator *self);