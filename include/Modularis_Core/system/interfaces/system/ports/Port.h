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

struct MDLRS_Port;

struct MDLRS_Any_port;
struct MDLRS_Port_group;

int
	MDLRS_Port_connect(struct MDLRS_Port *self, struct MDLRS_Any_port *port),
	MDLRS_Port_connect_port(struct MDLRS_Port *self, struct MDLRS_Port *port),
	MDLRS_Port_connect_group(struct MDLRS_Port *self, struct MDLRS_Port_group *group),
	MDLRS_Port_disconnect(struct MDLRS_Port *self),
	MDLRS_Port_disconnect_from_port(struct MDLRS_Port *self, struct MDLRS_Any_port *port),
	MDLRS_Port_disconnect_port(struct MDLRS_Port *self, struct MDLRS_Port *port),
	MDLRS_Port_disconnect_group(struct MDLRS_Port *self, struct MDLRS_Port_group *group),
	MDLRS_Port_disconnect_input(struct MDLRS_Port *self);
void MDLRS_Port_update(struct MDLRS_Port *self);
void MDLRS_Port_get_ready(struct MDLRS_Port *self);
struct MDLRS_Port *MDLRS_Port_connection(struct MDLRS_Port *self, unsigned connection);
unsigned MDLRS_Port_count(struct MDLRS_Port *self);
unsigned MDLRS_Port_find_connection(struct MDLRS_Port *self, struct MDLRS_Port *port);