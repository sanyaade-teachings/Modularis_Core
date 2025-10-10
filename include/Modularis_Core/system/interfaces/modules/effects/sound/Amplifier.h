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

struct MDLRS_Amplifier;

struct MDLRS_Modularis;
struct MDLRS_Sound;
struct MDLRS_Real_controller;

struct MDLRS_Amplifier *MDLRS_Amplifier_new(struct MDLRS_Modularis *project, float volume);
void MDLRS_Amplifier_init(struct MDLRS_Amplifier *self, struct MDLRS_Modularis *project, float volume);
void MDLRS_Amplifier_set_volume(struct MDLRS_Amplifier *self, float volume);
struct MDLRS_Sound *MDLRS_Amplifier_get_input(struct MDLRS_Amplifier *self);
struct MDLRS_Real_controller *MDLRS_Amplifier_get_volume(struct MDLRS_Amplifier *self);
struct MDLRS_Sound *MDLRS_Amplifier_get_output(struct MDLRS_Amplifier *self);
void MDLRS_Amplifier_deinit(struct MDLRS_Amplifier *self);
void MDLRS_Amplifier_remove(struct MDLRS_Amplifier *self);