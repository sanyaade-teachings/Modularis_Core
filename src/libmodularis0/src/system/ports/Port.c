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

#include <system/ports/Port.h>

#include <system/safe memory.h>
#include <system/ports/Connection.h>
#include <system/ports/Any_port.h>
#include <system/modules/Module.h>
#include <Modularis_Core/Modularis.h>
#include <string.h>
#include <stddef.h>
#include <ports/Port_group.h>

static const struct MDLRS_Any_port_f f=
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
};

void MDLRS_Port_init(struct MDLRS_Port *self, const char *type, struct MDLRS_Module *module)
{
	MDLRS_Any_port_init(&self->p, module);
	self->p.f=&f;

	self->type=type;
	self->connections=safe_array(module->project, "Connection @ MDLRS_Port_init()", sizeof(struct Connection), 8);
	self->connection_count=0;
}
int MDLRS_Port_connect(struct MDLRS_Port *self, struct MDLRS_Any_port *port)
{
	int result=((MDLRS_Any_port)port->f)->connect_port(port, self);
	if (self->p.module->index&&self->p.module->output_connections)
	{
		struct MDLRS_Module **modules=self->p.module->project->disconnected_modules;
		unsigned count=self->p.module->project->disconnected_module_count--;
		for (unsigned a=self->p.module->index; a!=count; a++)
		{
			modules[a]->index--;
			modules[a-1]=modules[a];
		}
		self->p.module->index=0;
	}
	return result;
}
int MDLRS_Port_connect_port(struct MDLRS_Port *self, struct MDLRS_Port *port)
{
	if (strcmp(self->type, port->type)) return 1;
	if (MDLRS_Port_find_connection(self, port)) return 2;
	struct MDLRS_Modularis *project=self->p.module->project;
	size_t count=safe_count(self->connections);
	if (self->connection_count==count) self->connections=safe_resize(project, self->connections, 2*count);
	self->connections[self->connection_count++]=(struct Connection){port, port->connection_count+1};
	count=safe_count(port->connections);
	if (port->connection_count==count) port->connections=safe_resize(project, port->connections, 2*count);
	port->connections[port->connection_count++]=(struct Connection){self, self->connection_count};
	port->p.module->output_connections++;
	return 0;
}
int MDLRS_Port_connect_group(struct MDLRS_Port *self, struct MDLRS_Port_group *group)
{
	for (unsigned a=0; a!=group->port_count; a++)
	{
		struct MDLRS_Any_port *port=group->ports[a];
		((MDLRS_Any_port)port->f)->connect(port, &self->p);
	}
	return 0;
}
int MDLRS_Port_disconnect(struct MDLRS_Port *self)
{
	if (self->connection_count)
	{
		unsigned count=self->connection_count;
		while (self->connection_count)
		{
			struct MDLRS_Port *port=self->connections[--self->connection_count].port;
			unsigned count=port->connection_count--;
			for (unsigned a=self->connections[self->connection_count].index; a!=count; a++)
			{
				port->connections[a].port->connections[port->connections[a].index-1].index--;
				port->connections[a-1]=port->connections[a];
			}
		}
		self->p.module->output_connections-=count;
		if (!self->p.module->output_connections)
		{
			struct MDLRS_Modularis *project=self->p.module->project;
			size_t count=safe_count(project->disconnected_modules);
			if (project->disconnected_module_count==count) project->disconnected_modules=safe_resize(project, project->disconnected_modules, 2*count);
			project->disconnected_modules[project->disconnected_module_count++]=self->p.module;
			self->p.module->index=project->disconnected_module_count;
		}
		return 0;
	}
	return 1;
}
int MDLRS_Port_disconnect_from_port(struct MDLRS_Port *self, struct MDLRS_Any_port *port)
{
	int result=((MDLRS_Any_port)port->f)->disconnect_port(port, self);
	if (!(self->p.module->index||self->p.module->output_connections))
	{
		struct MDLRS_Modularis *project=self->p.module->project;
		size_t count=safe_count(project->disconnected_modules);
		if (project->disconnected_module_count==count) project->disconnected_modules=safe_resize(project, project->disconnected_modules, 2*count);
		project->disconnected_modules[project->disconnected_module_count++]=self->p.module;
		self->p.module->index=project->disconnected_module_count;
	}
	return result;
}
int MDLRS_Port_disconnect_port(struct MDLRS_Port *self, struct MDLRS_Port *port)
{
	unsigned index=MDLRS_Port_find_connection(self, port);
	if (index)
	{
		unsigned port_index=self->connections[index-1].index;
		unsigned count=self->connection_count--;
		for (unsigned a=index; a!=count; a++)
		{
			self->connections[a].port->connections[self->connections[a].index-1].index--;
			self->connections[a-1]=self->connections[a];
		}
		count=port->connection_count--;
		for (unsigned a=port_index; a!=count; a++)
		{
			port->connections[a].port->connections[port->connections[a].index-1].index--;
			port->connections[a-1]=port->connections[a];
		}
		port->p.module->output_connections--;
		return 0;
	}
	return 1;
}
int MDLRS_Port_disconnect_group(struct MDLRS_Port *self, struct MDLRS_Port_group *group)
{
	for (unsigned a=0; a!=group->port_count; a++)
	{
		struct MDLRS_Any_port *port=group->ports[a];
		((MDLRS_Any_port)port->f)->disconnect_from_port(port, &self->p);
	}
	return 0;
}
int MDLRS_Port_disconnect_input(struct MDLRS_Port *self)
{
	if (self->connection_count)
	{
		while (self->connection_count) MDLRS_Port_disconnect_from_port(self->connections[self->connection_count-1].port, &self->p);
		return 0;
	}
	return 1;
}
void MDLRS_Port_update(struct MDLRS_Port *self)
{
	for (unsigned a=0; a!=self->connection_count; a++) MDLRS_Module_update(self->connections[a].port->p.module);
	((MDLRS_Port)self->p.f)->f2.on_update(self);
}
void MDLRS_Port_get_ready(struct MDLRS_Port *self)
{
	for (unsigned a=0; a!=self->connection_count; a++) MDLRS_Module_get_ready(self->connections[a].port->p.module);
}
struct MDLRS_Port *MDLRS_Port_connection(struct MDLRS_Port *self, unsigned connection)
{
	return self->connections[connection].port;
}
unsigned MDLRS_Port_count(struct MDLRS_Port *self)
{
	return self->connection_count;
}
unsigned MDLRS_Port_find_connection(struct MDLRS_Port *self, struct MDLRS_Port *port)
{
	for (unsigned a=0; a!=self->connection_count; a++) if (self->connections[a].port==port) return a+1;
	return 0;
}