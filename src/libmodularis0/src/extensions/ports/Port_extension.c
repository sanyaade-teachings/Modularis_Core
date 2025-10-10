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

#include <extensions/ports/Port_extension.h>

#include <system/safe memory.h>
#include <system/modules/Module.h>

static void on_update(struct Port_extension *self);
static const struct MDLRS_Port_f f=
{
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
		(void (*)(void *))MDLRS_Port_get_ready
	},
	{
		(void (*)(void *))on_update
	}
};

struct Port_extension *Port_extension_new(const char *type, struct MDLRS_Module *module, struct MDLRS_IPort *body)
{
	struct Port_extension *result=safe_object(module->project, "Port_extension", sizeof(struct Port_extension));
	MDLRS_Port_init(&result->p, type, module);
	result->p.p.f=&f;

	result->body=body;
	return result;
}
void on_update(struct Port_extension *self)
{
	self->body->f->on_update(self->body);
}
void Port_extension_remove(struct Port_extension *self)
{
	MDLRS_Port_deinit(&self->p);
	safe_dispose(self->p.p.module->project, self);
}