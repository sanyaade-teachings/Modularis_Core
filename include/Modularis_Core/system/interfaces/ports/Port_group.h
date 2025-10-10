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

struct MDLRS_Port_group;

struct MDLRS_Module;
struct MDLRS_Any_port;
struct MDLRS_Port;

struct MDLRS_Port_group *MDLRS_Port_group_new(struct MDLRS_Module *module);
void MDLRS_Port_group_init(struct MDLRS_Port_group *self, struct MDLRS_Module *module);
int
	MDLRS_Port_group_connect(struct MDLRS_Port_group *self, struct MDLRS_Any_port *port),
	MDLRS_Port_group_connect_port(struct MDLRS_Port_group *self, struct MDLRS_Port *port),
	MDLRS_Port_group_connect_group(struct MDLRS_Port_group *self, struct MDLRS_Port_group *group),
	MDLRS_Port_group_disconnect(struct MDLRS_Port_group *self),
	MDLRS_Port_group_disconnect_from_port(struct MDLRS_Port_group *self, struct MDLRS_Any_port *port),
	MDLRS_Port_group_disconnect_port(struct MDLRS_Port_group *self, struct MDLRS_Port *port),
	MDLRS_Port_group_disconnect_group(struct MDLRS_Port_group *self, struct MDLRS_Port_group *group),
	MDLRS_Port_group_disconnect_input(struct MDLRS_Port_group *self);
void MDLRS_Port_group_update(struct MDLRS_Port_group *self);
void MDLRS_Port_group_get_ready(struct MDLRS_Port_group *self);
void MDLRS_Port_group_add(struct MDLRS_Port_group *self, struct MDLRS_Any_port *port);
struct MDLRS_Any_port *MDLRS_Port_group_port(struct MDLRS_Port_group *self, unsigned port);
unsigned MDLRS_Port_group_count(struct MDLRS_Port_group *self);
void MDLRS_Port_group_deinit(struct MDLRS_Port_group *self);
void MDLRS_Port_group_remove(struct MDLRS_Port_group *self);