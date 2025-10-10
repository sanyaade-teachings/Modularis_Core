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

struct MDLRS_Port;
struct MDLRS_Module;

typedef const struct MDLRS_IPort_f *MDLRS_IPort;
struct MDLRS_IPort
{
	MDLRS_IPort f;
	struct MDLRS_Port *p;
};
struct MDLRS_IPort_f
{
	void (*on_update)(void *self);
};
void MDLRS_IPort_init(struct MDLRS_IPort *self, const char *type, struct MDLRS_Module *module);
void MDLRS_IPort_deinit(struct MDLRS_IPort *self);