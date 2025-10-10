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

struct MDLRS_Transpose;

struct MDLRS_Modularis;
struct MDLRS_Note;
struct MDLRS_Real_controller;

struct MDLRS_Transpose *MDLRS_Transpose_new(struct MDLRS_Modularis *project, float transposition);
void MDLRS_Transpose_init(struct MDLRS_Transpose *self, struct MDLRS_Modularis *project, float transposition);
void MDLRS_Transpose_set_transposition(struct MDLRS_Transpose *self, float transposition);
void MDLRS_Transpose_set_velocity(struct MDLRS_Transpose *self, float velocity);
struct MDLRS_Note *MDLRS_Transpose_get_input(struct MDLRS_Transpose *self);
struct MDLRS_Real_controller *MDLRS_Transpose_get_transposition(struct MDLRS_Transpose *self);
struct MDLRS_Real_controller *MDLRS_Transpose_get_velocity(struct MDLRS_Transpose *self);
struct MDLRS_Note *MDLRS_Transpose_get_output(struct MDLRS_Transpose *self);
void MDLRS_Transpose_deinit(struct MDLRS_Transpose *self);
void MDLRS_Transpose_remove(struct MDLRS_Transpose *self);