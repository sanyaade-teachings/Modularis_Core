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

struct MDLRS_Note;

struct MDLRS_Module;
struct MDLRS_Note_event;

struct MDLRS_Note *MDLRS_Note_new(struct MDLRS_Module *module);
void MDLRS_Note_init(struct MDLRS_Note *self, struct MDLRS_Module *module);
struct MDLRS_Note_event *MDLRS_Note_get_events(struct MDLRS_Note *self);
unsigned MDLRS_Note_get_event_count(struct MDLRS_Note *self);
unsigned MDLRS_Note_add_start(struct MDLRS_Note *self, float pitch, float velocity, float phase);
void MDLRS_Note_add_change(struct MDLRS_Note *self, unsigned scancode, float pitch, float velocity);
void MDLRS_Note_add_stop(struct MDLRS_Note *self, unsigned scancode);
void MDLRS_Note_clean(struct MDLRS_Note *self);
void MDLRS_Note_deinit(struct MDLRS_Note *self);
void MDLRS_Note_remove(struct MDLRS_Note *self);