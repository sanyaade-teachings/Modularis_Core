/*
(C) 2022, 2025 Серый MLGamer. All freedoms preserved.
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

#include <system/safe memory.h>

#include <Modularis_Core/system/Safe_block.h>
#include <stdlib.h>
#include <Modularis_Core/Modularis.h>

void *safe_object(struct MDLRS_Modularis *project, const char *name, size_t size)
{
	struct MDLRS_Safe_block *result=malloc(sizeof(struct MDLRS_Safe_block)+size);
	if (!result) return NULL;
	result->name=name;
	result->size=size;
	result->count=1;
	result->next=NULL;
	if (project->last_block)
	{
		result->previous=project->last_block;
		project->last_block->next=result;
	}
	else result->previous=NULL;
	project->last_block=result;
	if (project->block_count++==project->max_block_count) project->max_block_count++;
	project->allocated+=size;
	if (project->allocated>project->max_allocated) project->max_allocated=project->allocated;
	return (void *)(result+1);
}
void *safe_array(struct MDLRS_Modularis *project, const char *name, size_t size, size_t count)
{
	struct MDLRS_Safe_block *result=malloc(sizeof(struct MDLRS_Safe_block)+count*size);
	if (!result) return NULL;
	result->name=name;
	result->size=size;
	result->count=count;
	result->next=NULL;
	if (project->last_block)
	{
		result->previous=project->last_block;
		project->last_block->next=result;
	}
	else result->previous=NULL;
	project->last_block=result;
	if (project->block_count++==project->max_block_count) project->max_block_count++;
	project->allocated+=count*size;
	if (project->allocated>project->max_allocated) project->max_allocated=project->allocated;
	return (void *)(result+1);
}
void *safe_resize(struct MDLRS_Modularis *project, void *data, size_t new_count)
{
	struct MDLRS_Safe_block
		*result=data-sizeof(struct MDLRS_Safe_block),
		*previous=result->previous,
		*next=result->next;
	size_t size=result->size;
	size_t count=result->count;
	result=realloc(result, sizeof(struct MDLRS_Safe_block)+new_count*size);
	if (!result)
	{
		if (next)
		{
			if (previous)
			{
				previous->next=next;
				next->previous=previous;
			}
			else next->previous=NULL;
		}
		else if (previous)
		{
			previous->next=NULL;
			project->last_block=previous;
		}
		else project->last_block=NULL;
		project->block_count--;
		project->allocated-=count*size;
		return NULL;
	}
	project->allocated+=size*(new_count-count);
	if (project->allocated>project->max_allocated) project->max_allocated=project->allocated;
	if (next) next->previous=result;
	else project->last_block=result;
	if (previous) previous->next=result;
	result->count=new_count;
	return (void *)(result+1);
}
void safe_dispose(struct MDLRS_Modularis *project, void *data)
{
	struct MDLRS_Safe_block
		*block=data-sizeof(struct MDLRS_Safe_block),
		*previous=block->previous,
		*next=block->next;
	if (next)
	{
		if (previous)
		{
			previous->next=next;
			next->previous=previous;
		}
		else next->previous=NULL;
	}
	else if (previous)
	{
		previous->next=NULL;
		project->last_block=previous;
	}
	else project->last_block=NULL;
	project->block_count--;
	project->allocated-=block->count*block->size;
	free(block);
}