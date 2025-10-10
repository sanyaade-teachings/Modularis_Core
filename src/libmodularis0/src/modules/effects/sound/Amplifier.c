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

#include <modules/effects/sound/Amplifier.h>

#include <system/safe memory.h>

static void on_update(struct MDLRS_Amplifier *self);
static struct MDLRS_IModule_f f=
{
	(void (*)(void *))on_update
};

struct MDLRS_Amplifier *MDLRS_Amplifier_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Amplifier", sizeof(struct MDLRS_Amplifier));
}
struct MDLRS_Amplifier *MDLRS_Amplifier_new(struct MDLRS_Modularis *project, float volume)
{
	struct MDLRS_Amplifier *result=MDLRS_Amplifier_allocate(project);
	MDLRS_Amplifier_init(result, project, volume);
	return result;
}
void MDLRS_Amplifier_init(struct MDLRS_Amplifier *self, struct MDLRS_Modularis *project, float volume)
{
	MDLRS_Module_init(&self->p, project);
	self->p.f=&f;

	MDLRS_Sound_init(&self->input, &self->p);
	MDLRS_Real_controller_init(&self->volume, &self->p, volume);
	MDLRS_Sound_init(&self->output, &self->p);
	MDLRS_Port_group_add(&self->p.inputs, &self->input.p.p);
	MDLRS_Port_group_add(&self->p.inputs, &self->volume.p.p);
	MDLRS_Port_group_add(&self->p.outputs, &self->output.p.p);
}
void on_update(struct MDLRS_Amplifier *self)
{
	self->output.frame=self->volume.value*self->input.frame;
}
void MDLRS_Amplifier_set_volume(struct MDLRS_Amplifier *self, float volume)
{
	self->volume.value=volume;
}
struct MDLRS_Sound *MDLRS_Amplifier_get_input(struct MDLRS_Amplifier *self)
{
	return &self->input;
}
struct MDLRS_Real_controller *MDLRS_Amplifier_get_volume(struct MDLRS_Amplifier *self)
{
	return &self->volume;
}
struct MDLRS_Sound *MDLRS_Amplifier_get_output(struct MDLRS_Amplifier *self)
{
	return &self->output;
}
void MDLRS_Amplifier_deinit(struct MDLRS_Amplifier *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->input.p);
	MDLRS_Port_deinit(&self->volume.p);
	MDLRS_Port_deinit(&self->output.p);

	MDLRS_Module_deinit(&self->p);
}
void MDLRS_Amplifier_remove(struct MDLRS_Amplifier *self)
{
	MDLRS_Module_disconnect(&self->p);
	MDLRS_Port_deinit(&self->input.p);
	MDLRS_Port_deinit(&self->volume.p);
	MDLRS_Port_deinit(&self->output.p);

	MDLRS_Module_deinit(&self->p);
	safe_dispose(self->p.project, self);
}