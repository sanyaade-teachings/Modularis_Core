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

struct MDLRS_ADSR;

struct MDLRS_Module;
struct MDLRS_Real_controller;

enum MDLRS_ADSR_state
{
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE
};

struct MDLRS_ADSR *MDLRS_ADSR_new(struct MDLRS_Module *module, float attack, float decay, float sustain, float release);
void MDLRS_ADSR_init(struct MDLRS_ADSR *self, struct MDLRS_Module *module, float attack, float decay, float sustain, float release);
void
	MDLRS_ADSR_set_attack(struct MDLRS_ADSR *self, float attack),
	MDLRS_ADSR_set_decay(struct MDLRS_ADSR *self, float decay),
	MDLRS_ADSR_set_sustain(struct MDLRS_ADSR *self, float sustain),
	MDLRS_ADSR_set_release(struct MDLRS_ADSR *self, float release);
struct MDLRS_Real_controller
	*MDLRS_ADSR_get_attack(struct MDLRS_ADSR *self),
	*MDLRS_ADSR_get_decay(struct MDLRS_ADSR *self),
	*MDLRS_ADSR_get_sustain(struct MDLRS_ADSR *self),
	*MDLRS_ADSR_get_release(struct MDLRS_ADSR *self);
float MDLRS_ADSR_envelope(struct MDLRS_ADSR *self, enum MDLRS_ADSR_state state, float time);
void MDLRS_ADSR_deinit(struct MDLRS_ADSR *self);
void MDLRS_ADSR_remove(struct MDLRS_ADSR *self);