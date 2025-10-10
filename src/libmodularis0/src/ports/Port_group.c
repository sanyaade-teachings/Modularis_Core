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

#include <ports/Port_group.h>

#include <system/safe memory.h>
#include <system/modules/Module.h>
#include <system/ports/Any_port.h>
#include <system/ports/Port.h>
#include <stddef.h>

static const struct MDLRS_Any_port_f f=
{
	(int (*)(void *, struct MDLRS_Any_port *))MDLRS_Port_group_connect,
	(int (*)(void *, struct MDLRS_Port *))MDLRS_Port_group_connect_port,
	(int (*)(void *, struct MDLRS_Port_group *))MDLRS_Port_group_connect_group,
	(int (*)(void *))MDLRS_Port_group_disconnect,
	(int (*)(void *, struct MDLRS_Any_port *))MDLRS_Port_group_disconnect_from_port,
	(int (*)(void *, struct MDLRS_Port *))MDLRS_Port_group_disconnect_port,
	(int (*)(void *, struct MDLRS_Port_group *))MDLRS_Port_group_disconnect_group,
	(int (*)(void *))MDLRS_Port_group_disconnect_input,
	(void (*)(void *))MDLRS_Port_group_update,
	(void (*)(void *))MDLRS_Port_group_get_ready
};

struct MDLRS_Port_group *MDLRS_Port_group_allocate(struct MDLRS_Module *module)
{
	return safe_object(module->project, "MDLRS_Port_group", sizeof(struct MDLRS_Port_group));
}
struct MDLRS_Port_group *MDLRS_Port_group_new(struct MDLRS_Module *module)
{
	struct MDLRS_Port_group *result=MDLRS_Port_group_allocate(module);
	MDLRS_Port_group_init(result, module);
	return result;
}
void MDLRS_Port_group_init(struct MDLRS_Port_group *self, struct MDLRS_Module *module)
{
	MDLRS_Any_port_init(&self->p, module);
	self->p.f=&f;

	self->ports=safe_array(module->project, "MDLRS_Any_port * @ MDLRS_Port_group_init()", sizeof(struct MDLRS_Any_port *), 8);
	self->port_count=0;
}
int MDLRS_Port_group_connect(struct MDLRS_Port_group *self, struct MDLRS_Any_port *port)
{
	return ((MDLRS_Any_port)port->f)->connect_group(port, self);
}
int MDLRS_Port_group_connect_port(struct MDLRS_Port_group *self, struct MDLRS_Port *port)
{
	for (unsigned a=0; a!=self->port_count; a++) MDLRS_Port_connect(port, self->ports[a]);
	return 0;
}
int MDLRS_Port_group_connect_group(struct MDLRS_Port_group *self, struct MDLRS_Port_group *group)
{
	unsigned minimum_count=(self->port_count>group->port_count)*group->port_count+(self->port_count<=group->port_count)*self->port_count;
	for (unsigned a=0; a!=minimum_count; a++)
	{
		struct MDLRS_Any_port *port=group->ports[a];
		((MDLRS_Any_port)port->f)->connect(port, self->ports[a]);
	}
	return 0;
}
int MDLRS_Port_group_disconnect(struct MDLRS_Port_group *self)
{
	for (unsigned a=0; a!=self->port_count; a++)
	{
		struct MDLRS_Any_port *port=self->ports[a];
		((MDLRS_Any_port)port->f)->disconnect(port);
	}
	return 0;
}
int MDLRS_Port_group_disconnect_from_port(struct MDLRS_Port_group *self, struct MDLRS_Any_port *port)
{
	return ((MDLRS_Any_port)port->f)->disconnect_group(port, self);
}
int MDLRS_Port_group_disconnect_port(struct MDLRS_Port_group *self, struct MDLRS_Port *port)
{
	for (unsigned a=0; a!=self->port_count; a++) MDLRS_Port_disconnect_from_port(port, self->ports[a]);
	return 0;
}
int MDLRS_Port_group_disconnect_group(struct MDLRS_Port_group *self, struct MDLRS_Port_group *group)
{
	unsigned minimum_count=(self->port_count>group->port_count)*group->port_count+(self->port_count<=group->port_count)*self->port_count;
	for (unsigned a=0; a!=minimum_count; a++)
	{
		struct MDLRS_Any_port *port=group->ports[a];
		((MDLRS_Any_port)port->f)->disconnect_from_port(port, self->ports[a]);
	}
	return 0;
}
int MDLRS_Port_group_disconnect_input(struct MDLRS_Port_group *self)
{
	for (unsigned a=0; a!=self->port_count; a++)
	{
		struct MDLRS_Any_port *port=self->ports[a];
		((MDLRS_Any_port)port->f)->disconnect_input(port);
	}
	return 0;
}
void MDLRS_Port_group_update(struct MDLRS_Port_group *self)
{
	for (unsigned a=0; a!=self->port_count; a++)
	{
		struct MDLRS_Any_port *port=self->ports[a];
		((MDLRS_Any_port)port->f)->update(port);
	}
}
void MDLRS_Port_group_get_ready(struct MDLRS_Port_group *self)
{
	for (unsigned a=0; a!=self->port_count; a++)
	{
		struct MDLRS_Any_port *port=self->ports[a];
		((MDLRS_Any_port)port->f)->get_ready(port);
	}
}
void MDLRS_Port_group_add(struct MDLRS_Port_group *self, struct MDLRS_Any_port *port)
{
	size_t count=safe_count(self->ports);
	if (self->port_count==count) self->ports=safe_resize(self->p.module->project, self->ports, 2*count);
	self->ports[self->port_count++]=port;
}
struct MDLRS_Any_port *MDLRS_Port_group_port(struct MDLRS_Port_group *self, unsigned port)
{
	return self->ports[port];
}
unsigned MDLRS_Port_group_count(struct MDLRS_Port_group *self)
{
	return self->port_count;
}
void MDLRS_Port_group_deinit(struct MDLRS_Port_group *self)
{
	safe_dispose(self->p.module->project, self->ports);
}
void MDLRS_Port_group_remove(struct MDLRS_Port_group *self)
{
	safe_dispose(self->p.module->project, self->ports);
	safe_dispose(self->p.module->project, self);
}