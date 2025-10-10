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

#include <ports/controllers/ADSR.h>

#include <system/safe memory.h>
#include <system/modules/Module.h>

struct MDLRS_ADSR *MDLRS_ADSR_allocate(struct MDLRS_Module *module)
{
	return safe_object(module->project, "MDLRS_ADSR", sizeof(struct MDLRS_ADSR));
}
struct MDLRS_ADSR *MDLRS_ADSR_new(struct MDLRS_Module *module, float attack, float decay, float sustain, float release)
{
	struct MDLRS_ADSR *result=MDLRS_ADSR_allocate(module);
	MDLRS_ADSR_init(result, module, attack, decay, sustain, release);
	return result;
}
void MDLRS_ADSR_init_body(struct MDLRS_ADSR *self, struct MDLRS_Module *module, float attack, float decay, float sustain, float release)
{
	MDLRS_Real_controller_init(&self->attack, module, attack);
	MDLRS_Real_controller_init(&self->decay, module, decay);
	MDLRS_Real_controller_init(&self->sustain, module, sustain);
	MDLRS_Real_controller_init(&self->release, module, release);
	MDLRS_Port_group_add(&self->p, &self->attack.p.p);
	MDLRS_Port_group_add(&self->p, &self->decay.p.p);
	MDLRS_Port_group_add(&self->p, &self->sustain.p.p);
	MDLRS_Port_group_add(&self->p, &self->release.p.p);
}
void MDLRS_ADSR_init(struct MDLRS_ADSR *self, struct MDLRS_Module *module, float attack, float decay, float sustain, float release)
{
	MDLRS_Port_group_init(&self->p, module);
	MDLRS_ADSR_init_body(self, module, attack, decay, sustain, release);
}
void MDLRS_ADSR_set_attack(struct MDLRS_ADSR *self, float attack)
{
	self->attack.value=attack;
}
void MDLRS_ADSR_set_decay(struct MDLRS_ADSR *self, float decay)
{
	self->decay.value=decay;
}
void MDLRS_ADSR_set_sustain(struct MDLRS_ADSR *self, float sustain)
{
	self->sustain.value=sustain;
}
void MDLRS_ADSR_set_release(struct MDLRS_ADSR *self, float release)
{
	self->release.value=release;
}
struct MDLRS_Real_controller *MDLRS_ADSR_get_attack(struct MDLRS_ADSR *self)
{
	return &self->attack;
}
struct MDLRS_Real_controller *MDLRS_ADSR_get_decay(struct MDLRS_ADSR *self)
{
	return &self->decay;
}
struct MDLRS_Real_controller *MDLRS_ADSR_get_sustain(struct MDLRS_ADSR *self)
{
	return &self->sustain;
}
struct MDLRS_Real_controller *MDLRS_ADSR_get_release(struct MDLRS_ADSR *self)
{
	return &self->release;
}
float MDLRS_ADSR_envelope(struct MDLRS_ADSR *self, enum MDLRS_ADSR_state state, float time)
{
	switch (state)
	{
		case ATTACK: return time/self->attack.value;
		case DECAY: return (self->sustain.value*time+self->decay.value-time)/self->decay.value;
		case SUSTAIN: return self->sustain.value;
		case RELEASE: return self->sustain.value*(self->release.value-time)/self->release.value;
	}
}
void MDLRS_ADSR_deinit(struct MDLRS_ADSR *self)
{
	MDLRS_Port_deinit(&self->attack.p);
	MDLRS_Port_deinit(&self->decay.p);
	MDLRS_Port_deinit(&self->sustain.p);
	MDLRS_Port_deinit(&self->release.p);

	MDLRS_Port_group_deinit(&self->p);
}
void MDLRS_ADSR_remove(struct MDLRS_ADSR *self)
{
	MDLRS_ADSR_deinit(self);
	safe_dispose(self->p.p.module->project, self);
}