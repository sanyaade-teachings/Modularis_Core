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

struct MDLRS_Modulator;

struct MDLRS_Modularis;
struct MDLRS_Sound;

struct MDLRS_Modulator *MDLRS_Modulator_new(struct MDLRS_Modularis *project);
void MDLRS_Modulator_init(struct MDLRS_Modulator *self, struct MDLRS_Modularis *project);
struct MDLRS_Sound *MDLRS_Modulator_get_carrier(struct MDLRS_Modulator *self);
struct MDLRS_Sound *MDLRS_Modulator_get_modulator(struct MDLRS_Modulator *self);
struct MDLRS_Sound *MDLRS_Modulator_get_output(struct MDLRS_Modulator *self);
void MDLRS_Modulator_deinit(struct MDLRS_Modulator *self);
void MDLRS_Modulator_remove(struct MDLRS_Modulator *self);