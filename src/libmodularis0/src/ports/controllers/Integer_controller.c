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

#include <ports/controllers/Integer_controller.h>

#include <stdint.h>
#include <system/safe memory.h>
#include <system/modules/Module.h>
#include <system/ports/Connection.h>

static void on_update(struct MDLRS_Integer_controller *self);
static const struct MDLRS_Port_f f=
{
	(int (*)(void *, struct MDLRS_Any_port *))MDLRS_Port_connect,
	(int (*)(void *, struct MDLRS_Port *))MDLRS_Port_connect_port,
	(int (*)(void *, struct MDLRS_Port_group *))MDLRS_Port_connect_group,
	(int (*)(void *))MDLRS_Port_disconnect,
	(int (*)(void *, struct MDLRS_Any_port *))MDLRS_Port_disconnect_from_port,
	(int (*)(void *, struct MDLRS_Port *))MDLRS_Port_disconnect_port,
	(int (*)(void *, struct MDLRS_Port_group *))MDLRS_Port_disconnect_group,
	(int (*)(void *))MDLRS_Port_disconnect_input,
	(void (*)(void *))MDLRS_Port_update,
	(void (*)(void *))MDLRS_Port_get_ready,
	(void (*)(void *))on_update
};

struct MDLRS_Integer_controller *MDLRS_Integer_controller_allocate(struct MDLRS_Module *module)
{
	return safe_object(module->project, "MDLRS_Integer_controller", sizeof(struct MDLRS_Integer_controller));
}
struct MDLRS_Integer_controller *MDLRS_Integer_controller_new(struct MDLRS_Module *module, int32_t value)
{
	struct MDLRS_Integer_controller *result=MDLRS_Integer_controller_allocate(module);
	MDLRS_Port_init(&result->p, "Integer_controller", module);
	result->p.p.f=&f;

	result->value=value;
	return result;
}
void MDLRS_Integer_controller_init(struct MDLRS_Integer_controller *self, struct MDLRS_Module *module, int32_t value)
{
	MDLRS_Port_init(&self->p, "Integer_controller", module);
	self->p.p.f=&f;

	self->value=value;
}
void on_update(struct MDLRS_Integer_controller *self)
{
	unsigned connection_count=self->p.connection_count;
	if (connection_count)
	{
		int32_t value=0;
		struct Connection *connections=self->p.connections;
		for (unsigned a=0; a!=connection_count; a++) value+=((struct MDLRS_Integer_controller *)connections[a].port)->value;
		self->value=value/connection_count;
	}
}
void MDLRS_Integer_controller_set(struct MDLRS_Integer_controller *self, int32_t value)
{
	self->value=value;
}
int32_t MDLRS_Integer_controller_get(struct MDLRS_Integer_controller *self)
{
	return self->value;
}
void MDLRS_Integer_controller_deinit(struct MDLRS_Integer_controller *self)
{
	MDLRS_Port_deinit(&self->p);
}
void MDLRS_Integer_controller_remove(struct MDLRS_Integer_controller *self)
{
	MDLRS_Port_deinit(&self->p);
	safe_dispose(self->p.p.module->project, self);
}