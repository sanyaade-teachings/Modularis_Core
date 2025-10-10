/*
(C) 2024-2025 Серый MLGamer. All freedoms preserved.
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

#include <modules/effects/sound/Modulator.h>

#include <system/safe memory.h>

static void on_update(struct MDLRS_Modulator *self);
static struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Modulator *MDLRS_Modulator_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Modulator", sizeof(struct MDLRS_Modulator));
}
struct MDLRS_Modulator *MDLRS_Modulator_new(struct MDLRS_Modularis *project)
{
	struct MDLRS_Modulator *result=MDLRS_Modulator_allocate(project);
	MDLRS_Modulator_init(result, project);
	return result;
}
void MDLRS_Modulator_init(struct MDLRS_Modulator *self, struct MDLRS_Modularis *project)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Sound_init(&self->carrier, &self->p);
	MDLRS_Sound_init(&self->modulator, &self->p);
	MDLRS_Sound_init(&self->output, &self->p);
	MDLRS_Port_group_add(&self->p.inputs, &self->carrier.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->modulator.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
}
void on_update(struct MDLRS_Modulator *self)
{
	if (self->modulator.p.connection_count) self->output.frame=self->modulator.frame*self->carrier.frame;
	else self->output.frame=self->carrier.frame;
}
struct MDLRS_Sound *MDLRS_Modulator_get_carrier(struct MDLRS_Modulator *self)
{
	return &self->carrier;
}
struct MDLRS_Sound *MDLRS_Modulator_get_modulator(struct MDLRS_Modulator *self)
{
	return &self->modulator;
}
struct MDLRS_Sound *MDLRS_Modulator_get_output(struct MDLRS_Modulator *self)
{
	return &self->output;
}
void MDLRS_Modulator_deinit(struct MDLRS_Modulator *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->carrier.p);
	MDLRS_Port_deinit(&self->modulator.p);
	MDLRS_Port_deinit(&self->output.p);

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Modulator_remove(struct MDLRS_Modulator *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->carrier.p);
	MDLRS_Port_deinit(&self->modulator.p);
	MDLRS_Port_deinit(&self->output.p);

	MDLRS_Module_deinit(&self->p);
	safe_dispose(self->p.project, self);
}