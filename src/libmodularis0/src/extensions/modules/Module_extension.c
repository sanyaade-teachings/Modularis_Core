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

#include <extensions/modules/Module_extension.h>

#include <system/safe memory.h>
#include <system/ports/Port.h>

static void on_update(struct Module_extension *self);
static const struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct Module_extension *Module_extension_new(struct MDLRS_Modularis *project, struct MDLRS_IModule *body)
{
	struct Module_extension *result=safe_object(project, "Module_extension", sizeof(struct Module_extension));
	MDLRS_Module_init(&result->p, project);
	result->p.f=&f;

	result->body=body;
	return result;
}
void on_update(struct Module_extension *self)
{
	self->body->f->on_update(self->body);
}
void Module_extension_remove(struct Module_extension *self)
{
	MDLRS_Module_deinit(&self->p);
	safe_dispose(self->p.project, self);
}