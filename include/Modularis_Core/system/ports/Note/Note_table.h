/*
(C) 2025 Серый MLGamer. All freedoms preserved.
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

struct MDLRS_Note_table;

struct MDLRS_Modularis;

struct MDLRS_Note_table *MDLRS_Note_table_new(struct MDLRS_Modularis *project);
void MDLRS_Note_table_init(struct MDLRS_Note_table *self, struct MDLRS_Modularis *project);
void
	MDLRS_Note_table_set(struct MDLRS_Note_table *self, unsigned scancode, void *data),
	*MDLRS_Note_table_get(struct MDLRS_Note_table *self, unsigned scancode),
	MDLRS_Note_table_unset(struct MDLRS_Note_table *self, unsigned scancode);
void MDLRS_Note_table_deinit(struct MDLRS_Note_table *self);
void MDLRS_Note_table_remove(struct MDLRS_Note_table *self);