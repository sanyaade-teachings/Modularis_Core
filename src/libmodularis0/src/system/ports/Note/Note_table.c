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

#include <system/ports/Note/Note_table.h>

#include <system/safe memory.h>
#include <stdbool.h>

struct MDLRS_Note_table *MDLRS_Note_table_allocate(struct MDLRS_Modularis *project)
{
	return safe_object(project, "MDLRS_Note_table", sizeof(struct MDLRS_Note_table));
}
struct MDLRS_Note_table *MDLRS_Note_table_new(struct MDLRS_Modularis *project)
{
	struct MDLRS_Note_table *result=MDLRS_Note_table_allocate(project);
	MDLRS_Note_table_init(result, project);
	return result;
}
void MDLRS_Note_table_init(struct MDLRS_Note_table *self, struct MDLRS_Modularis *project)
{
	self->project=project;
	self->elements=safe_array(project, "MDLRS_Note_table_element @ MDLRS_Note_table_init()", sizeof(struct MDLRS_Note_table_element), 8);
	self->count=0;
	safe_clean(self->elements);
}
void MDLRS_Note_table_set(struct MDLRS_Note_table *self, unsigned scancode, void *data)
{
	struct MDLRS_Note_table_element *elements=self->elements;
	unsigned size=safe_count(elements);
	unsigned a;
	for (a=scancode%size; elements[a].exists; a=(a+1)%size);
	if (4*++self->count/size!=3)
	{
		elements[a]=(struct MDLRS_Note_table_element){true, scancode, data};
		return;
	}
	unsigned new_size=2*size;
	struct MDLRS_Note_table_element *new_elements=safe_array(self->project, "MDLRS_Note_table_element @ MDLRS_Note_table_set()", sizeof(struct MDLRS_Note_table_element), new_size);
	safe_clean(new_elements);
	for (unsigned b=0; b!=size; b++)
	{
		if (!elements[b].exists) continue;
		for (a=elements[b].scancode%new_size; new_elements[a].exists; a=(a+1)%new_size);
		new_elements[a]=elements[b];
	}
	safe_dispose(self->project, elements);
	self->elements=new_elements;
	for (a=scancode%new_size; new_elements[a].exists; a=(a+1)%new_size);
	new_elements[a]=(struct MDLRS_Note_table_element){true, scancode, data};
}
void *MDLRS_Note_table_get(struct MDLRS_Note_table *self, unsigned scancode)
{
	struct MDLRS_Note_table_element *elements=self->elements;
	unsigned size=safe_count(elements);
	for (unsigned a=scancode%size; true; a=(a+1)%size) if (elements[a].exists) if (elements[a].scancode==scancode) return elements[a].data;
}
void MDLRS_Note_table_unset(struct MDLRS_Note_table *self, unsigned scancode)
{
	struct MDLRS_Note_table_element *elements=self->elements;
	unsigned size=safe_count(elements);
	for (unsigned a=scancode%size; true; a=(a+1)%size) if (elements[a].exists) if (elements[a].scancode==scancode)
	{
		elements[a].exists=false;
		self->count--;
		return;
	}
}
void MDLRS_Note_table_deinit(struct MDLRS_Note_table *self)
{
	safe_dispose(self->project, self->elements);
}
void MDLRS_Note_table_remove(struct MDLRS_Note_table *self)
{
	safe_dispose(self->project, self->elements);
	safe_dispose(self->project, self);
}
void MDLRS_Note_table_dispose(struct MDLRS_Note_table *self)
{
	safe_dispose(self->project, self);
}